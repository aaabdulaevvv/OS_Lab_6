#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

const int NUM_ITERATIONS = 10000000;
int v = 0;
mutex m;

void increment_v() {
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        m.lock();
        ++v;
        m.unlock();
    }
}

int main() {
    thread t1(increment_v);
    thread t2(increment_v);

    t1.join();
    t2.join();

    cout << "v = " << v << endl;

    return 0;
}
