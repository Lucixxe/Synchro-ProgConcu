#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <cassert>

const int NUM_THREADS = 8;
const int ITERATIONS = 1000000;

namespace Sync {
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
    }

    // 1. Mutex
    std::mutex mtx;
    void mutex_increment(int& counter) {
        std::lock_guard<std::mutex> lock(mtx);
        ++counter;
    }

    // 2. Spinlock (Test-and-Test-and-Set)
    std::atomic_flag spinlock_flag = ATOMIC_FLAG_INIT;
    void spinlock_increment(int& counter) {
        while (spinlock_flag.test_and_set(std::memory_order_acquire));
        ++counter;
        spinlock_flag.clear(std::memory_order_release);
    }

    // 3. Atomic CAS
    std::atomic<int> atomic_counter{0};
    void cas_increment(int&) {
        int old, desired;
        do {
            old = atomic_counter.load();
            desired = old + 1;
        } while (!atomic_counter.compare_exchange_weak(old, desired));
    }

    // 4. Dekker’s Algorithm (simplified 2-thread version only)
    // Skipped here for NUM_THREADS > 2, can be added as separate test
}

int main() {
    Sync::benchmark("Mutex", Sync::mutex_increment);
    Sync::benchmark("Spinlock", Sync::spinlock_increment);
    Sync::benchmark("CAS (lock-free)", Sync::cas_increment);

    // Note: Dekker and MCS require special handling due to their structure.
    return 0;
}
