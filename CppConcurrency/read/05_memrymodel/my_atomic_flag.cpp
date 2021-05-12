/*
 * @Author: your name
 * @Date: 2021-02-08 15:28:02
 * @LastEditTime: 2021-02-22 14:03:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/my_atomic_flag.cpp
 */
#include <iostream>              // std::cout
#include <atomic>                // std::atomic, std::atomic_flag, ATOMIC_FLAG_INIT
#include <thread>                // std::thread, std::this_thread::yield
#include <vector>                // std::vector





/*
void yield() noexcept; 
其作用是当前线程“放弃”执行，让操作系统调度另一线程继续执行
比如说你的线程需要等待某个操作完成，如果你直接用一个循环不断判断这个操作是否完成就会使得这个线程占满CPU时间，这会造成资源浪费。
这时候你可以判断一次操作是否完成，如果没有完成就调用yield交出时间片，过一会儿再来判断是否完成，这样这个线程占用CPU时间会大大减少
*/

std::atomic<bool> ready(false);    // can be checked without being set

//std::atomic_flag类型的对象必须被ATOMIC_FLAG_INIT初始化。
//初始化标志位是“清除”状态。这里没得选择，这个标志总是初始化为“清除”
std::atomic_flag winner = ATOMIC_FLAG_INIT;    // always set when checked

void count1m(int id)
{
    while (!ready) {
        std::this_thread::yield();
    } // 等待主线程中设置 ready 为 true.

    for (int i = 0; i < 1000000; ++i) {
    } // 计数.



/*
test_and_set() 函数检查 std::atomic_flag 标志，如果 std::atomic_flag 之前没有被设置过，
则设置 std::atomic_flag 的标志，并返回先前该 std::atomic_flag 对象是否被设置过，
    如果之前 std::atomic_flag 对象已被设置，则返回 true，否则返回 false。
test-and-set 操作是原子的（因此 test-and-set 是原子 read-modify-write （RMW）操作）。
test_and_set 可以指定 Memory Order(后续的文章会详细介绍 C++11 的 Memory Order，此处为了完整性列出 test_and_set 参数 sync 的取值)，取值如下：
*/
    // 如果某个线程率先执行完上面的计数过程，则输出自己的 ID.
    // 此后其他线程执行 test_and_set 是 if 语句判断为 false，
    // 因此不会输出自身 ID.
    if (!winner.test_and_set()) {
        std::cout << "thread #" << id << " won!\n";
    }
};

int main()
{
    std::vector<std::thread> threads;
    std::cout << "spawning 10 threads that count to 1 million...\n";
    for (int i = 1; i <= 10; ++i)
    {
        threads.push_back(std::thread(count1m, i));
    }

    ready = true;

    for (auto & th:threads)
        th.join();

    return 0;
}