
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <cassert>
#include <fstream>

int NUM_THREADS = 8;
int ITERATIONS = 1000000;

namespace Sync {

    std::mutex mtx;
    void mutex_increment(int& counter) {
        std::lock_guard<std::mutex> lock(mtx);
        ++counter;
    }

    std::atomic_flag spinlock_flag = ATOMIC_FLAG_INIT;
    void spinlock_increment(int& counter) {
        int attempts = 0;
        while (spinlock_flag.test_and_set(std::memory_order_acquire)) {
            if (++attempts > 100)
                std::this_thread::yield(); // Laisse le CPU à un autre thread après 100 essais
        }
        ++counter;
        spinlock_flag.clear(std::memory_order_release);
    }    

    void cas_increment(int& counter) {
        std::atomic<int>* ptr = reinterpret_cast<std::atomic<int>*>(&counter);
        int old, desired;
        int attempts = 0;
        do {
            old = ptr->load();
            desired = old + 1;
            if (++attempts > 100)
                std::this_thread::yield(); // Backoff après trop de conflits
        } while (!ptr->compare_exchange_weak(old, desired));
    }    

    void naive_increment(int& counter) {
        ++counter;
    }

    void benchmark(const std::string& name, void(*sync_func)(int&)) {
        int counter = 0;
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back([&counter, sync_func]() {
                for (int j = 0; j < ITERATIONS; ++j)
                    sync_func(counter);
            });
        }

        for (auto& t : threads) t.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(end - start).count();

        std::cout << name << " - Final counter: " << counter
                  << " - Time: " << duration << "s\n";

        static std::ofstream csv("benchmark.csv", std::ios::out | std::ios::trunc);
        static bool header_written = false;
        if (!header_written) {
            csv << "method,counter,time_s\n";
            header_written = true;
        }
        csv << name << "," << counter << "," << duration << "\n";
    }

    void run_all_benchmarks(const std::vector<int>& thread_counts, int iterations_per_thread) {
        std::ofstream csv("benchmark_scaling.csv", std::ios::out | std::ios::trunc);
        csv << "method,num_threads,counter,time_s\n";
    
        auto methods = std::vector<std::pair<std::string, void(*)(int&)>>{
            {"Naive", naive_increment},
            {"Mutex", mutex_increment},
            {"Spinlock", spinlock_increment},
            {"CAS", cas_increment}
        };
    
        for (int threads : thread_counts) {
            for (auto& [name, func] : methods) {
                int counter = 0;
                auto start = std::chrono::high_resolution_clock::now();
    
                std::vector<std::thread> workers;
                for (int i = 0; i < threads; ++i) {
                    workers.emplace_back([&counter, func, iterations_per_thread]() {
                        for (int j = 0; j < iterations_per_thread; ++j)
                            func(counter);
                    });
                }
    
                for (auto& t : workers) t.join();
    
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration<double>(end - start).count();
    
                std::cout << name << " [" << threads << " threads]"
                          << " - Final counter: " << counter
                          << " - Time: " << duration << "s\n";
    
                csv << name << "," << threads << "," << counter << "," << duration << "\n";
            }
        }
    }
    

} // end namespace Sync

int main(int argc, char* argv[]) {
    int iterations = (argc >= 2) ? std::stoi(argv[1]) : 1000000;

    std::vector<int> thread_counts = {1, 2, 4, 8, 16};
    std::cout << "Benchmarking with " << iterations << " iterations per thread\n";
    Sync::run_all_benchmarks(thread_counts, iterations);

    return 0;
}

