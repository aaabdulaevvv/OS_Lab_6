#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> shared_variable(0);

void increment_variable(int num_iterations) {
    for (int i = 0; i < num_iterations; ++i) {
        shared_variable.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    const int num_threads = 2;
    const int num_iterations = 10000000;
    std::thread threads[num_threads];

    // Start threads
    for (int i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(increment_variable, num_iterations);
    }

    // Join threads
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    // Print final value of shared variable
    std::cout << "Final value of shared variable: " << shared_variable << std::endl;

    return 0;
}
