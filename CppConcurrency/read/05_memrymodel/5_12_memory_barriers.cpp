/*
 * @Author: your name
 * @Date: 2021-02-23 14:51:29
 * @LastEditTime: 2021-02-23 14:59:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5_12_memory_barriers.cpp
 */
//
//  5.12.cpp 同步操作和强制排序->栅栏
//  Cpp-Concurrency
/*
    释放栅栏②与获取栅栏⑤同步，这是因为加载y的操作④读取③处存储的值。
    所以，①处存储x先行于⑥处加载x，最后x读取出来必为true，并且断言不会被触发⑦。
    原先不带栅栏的存储和加载x是无序的，并且断言是可能会触发。
    这两个栅栏都是必要的：需要在一个线程中进行释放，然后在另一个线程中进行获取，这样才能构建出同步关系。
*/
//  栅栏可以让自由操作变得有序

#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);   //1
    std::atomic_thread_fence(std::memory_order_release); //2 释放栅栏
    y.store(true, std::memory_order_relaxed);   //3
}

void read_y_then_x() {
    while (!y.load(std::memory_order_relaxed));    //4
    std::atomic_thread_fence(std::memory_order_acquire); //5 获取栅栏
    if (x.load(std::memory_order_relaxed))    //6
        ++z;
}

int main() {
    x = false;
    y = false;
    z = 0;
    
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0); //7 因为栅栏的存在，断言不会被触发
}
