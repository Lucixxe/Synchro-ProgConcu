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
                std::this_thread::yield();
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
                std::this_thread::yield();
        } while (!ptr->compare_exchange_weak(old, desired));
    }

    void naive_increment(int& counter) {
        ++counter;
    }

    void run_all_benchmarks(const std::vector<int>& thread_counts, int iterations_per_thread) {
        std::ofstream csv("benchmark_scaling.csv", std::ios::out | std::ios::trunc);
        csv << "method,num_threads,counter,time_s,correct\n";

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

                int expected = threads * iterations_per_thread;
                bool correct = (counter == expected);

                if (!correct) {
                    std::cerr << "⚠️  METHODE " << name << " [" << threads << " threads] : ERREUR - attendu "
                              << expected << ", obtenu " << counter << "\n";
                }

                std::cout << name << " [" << threads << " threads]"
                          << " - Final counter: " << counter
                          << " - Time: " << duration << "s"
                          << " - Correct: " << (correct ? "YES" : "NO") << "\n";

                csv << name << "," << threads << "," << counter << "," << duration << "," << (correct ? "1" : "0") << "\n";
            }
        }
    }

    void run_iteration_benchmarks(int num_threads, const std::vector<int>& iteration_counts) {
        std::ofstream csv("benchmark_iterations.csv", std::ios::out | std::ios::trunc);
        csv << "method,num_threads,iterations,counter,time_s,correct\n";

        auto methods = std::vector<std::pair<std::string, void(*)(int&)>>{
            {"Naive", naive_increment},
            {"Mutex", mutex_increment},
            {"Spinlock", spinlock_increment},
            {"CAS", cas_increment}
        };

        for (int iters : iteration_counts) {
            for (auto& [name, func] : methods) {
                int counter = 0;
                auto start = std::chrono::high_resolution_clock::now();

                std::vector<std::thread> workers;
                for (int i = 0; i < num_threads; ++i) {
                    workers.emplace_back([&counter, func, iters]() {
                        for (int j = 0; j < iters; ++j)
                            func(counter);
                    });
                }

                for (auto& t : workers) t.join();

                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration<double>(end - start).count();

                int expected = num_threads * iters;
                bool correct = (counter == expected);

                if (!correct) {
                    std::cerr << "⚠️  METHODE " << name << " [" << iters << " iterations] : ERREUR - attendu "
                              << expected << ", obtenu " << counter << "\n";
                }

                std::cout << name << " [" << iters << " iterations x " << num_threads << " threads]"
                          << " - Final counter: " << counter
                          << " - Time: " << duration << "s"
                          << " - Correct: " << (correct ? "YES" : "NO") << "\n";

                csv << name << "," << num_threads << "," << iters << "," << counter << "," << duration << "," << (correct ? "1" : "0") << "\n";
            }
        }
    }

} // namespace Sync

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage:\n"
                  << "  ./benchmark.exe threads <iterations_per_thread>\n"
                  << "  ./benchmark.exe iterations <num_threads>\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "threads") {
        int iterations = std::stoi(argv[2]);
        std::vector<int> thread_counts = {1, 2, 4, 8, 16};
        std::cout << "Mode: threads\n";
        Sync::run_all_benchmarks(thread_counts, iterations);
    } else if (mode == "iterations") {
        int num_threads = std::stoi(argv[2]);
        std::vector<int> iteration_counts = {100000, 500000, 1000000, 5000000, 10000000};
        std::cout << "Mode: iterations\n";
        Sync::run_iteration_benchmarks(num_threads, iteration_counts);
    } else {
        std::cerr << "Unknown mode. Use 'threads' or 'iterations'.\n";
        return 1;
    }

    return 0;
}