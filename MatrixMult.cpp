#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

using namespace std;

const int MAX_SIZE = 1000;

int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
mutex mtx;

void multiply_vectors(int i, int j, int m) {
    int sum = 0;
    for (int p = 0; p < m; p++) {
        sum += A[i][p] * B[p][j];
    }
    mtx.lock(); // Блокуємо доступ до загальних ресурсів
    C[i][j] = sum;
    mtx.unlock(); // Розблоковуємо доступ до загальних ресурсів
    //cout << "[" << i << "," << j << "]=" << C[i][j] << endl;
}

void multiply_matrices(int n, int m, int k, int num_threads) {
    vector<thread> threads;
    // Створюємо num_threads потоків для обчислення матриці-результату
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            threads.emplace_back(multiply_vectors, i, j, m);
            if (threads.size() == num_threads) {
                // Якщо досягли максимальної кількості потоків, то чекаємо їх завершення
                for (auto &t : threads) {
                    t.join();
                }
                threads.clear();
            }
        }
    }
    // Чекаємо завершення усіх потоків
    for (auto &t : threads) {
        t.join();
    }
}

int main() {
    int n = 100, m = 100, k = 100;

    // Заповнюємо матриці A та B довільними значеннями
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A[i][j] = i * m + j;
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            B[i][j] = i * k + j;
        }
    }

    int num_threads = 10000;
    auto start_time = chrono::high_resolution_clock::now();
    multiply_matrices(n, m, k, num_threads);
    auto end_time = chrono::high_resolution_clock::now();
    // Обчислюємо час виконання та виводимо результат
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
    cout << "Execution time with " << num_threads << " threads: " << duration << " microseconds" << endl;

    return 0;
}

/* Будемо множити матриці 100*100 з різною кількістю потоків (по 5 разів) та записувати середній час виконання (в секундах):
1 - 0.684
2 - 0.585
3 - 0.525
4 - 0.496
5 - 0.478
6 - 0.464
7 - 0.454
8 - 0.447
10 - 0.430
50 - 0.410
100 - 0.409
1000 - 0.441
10000 - 0.456
*/
