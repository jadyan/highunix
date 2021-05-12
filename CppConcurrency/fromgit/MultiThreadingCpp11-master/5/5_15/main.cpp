#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
using namespace std;

bool x, z;
atomic<bool> y;

void write_x_then_y() {
    x = true;
    atomic_thread_fence(memory_order_release);
    y.store(true, memory_order_relaxed);
}

void read_y_then_x() {
    while(!y.load(memory_order_relaxed));
    atomic_thread_fence(memory_order_acquire);
    if(x) {
        z++;
    }
}

int main() {
    x = y = z = false;

    thread a(write_x_then_y);
    thread b(read_y_then_x);

    a.join(); b.join();

    cout << "z = always 1: " << z << endl;
}
