
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

/*
https://blog.csdn.net/qccz123456/article/details/81329261
*/
 
class spin_mutex {
    std::atomic<bool> flag = ATOMIC_VAR_INIT(false);
public:
    spin_mutex() = default;
    spin_mutex(const spin_mutex&) = delete;
    spin_mutex& operator= (const spin_mutex&) = delete;
    void lock() {
        bool expected = false;
        while (!flag.compare_exchange_strong(expected, true))
            expected = false;
    }
    void unlock() {
        flag.store(false);
    }
};
 
long size = 1000000;
long total = 0;
std::atomic_long total2(0);
std::mutex m;
spin_mutex lock;
 
void thread_click()
{
    for (int i = 0; i < size; ++i)
    {
        ++total;
    }
}
 
void mutex_click()
{
    for (int i = 0; i < size; ++i)
    {
        m.lock();
        ++total;
        m.unlock();
    }
}
 
void atomic_click()
{
    for (int i = 0; i < size; ++i)
    {
        ++total2;
    }
}
 
 
void spinlock_click()
{
    for (int i = 0; i < size; ++i)
    {
        lock.lock();
        ++total;
        lock.unlock();
    }
}
 
int main()
{
    int thnum = 100;
    std::vector<std::thread> threads(thnum);
    clock_t start, end;
 
    total = 0;
    start = clock();
    for (int i = 0; i < size * thnum; i++) {
        ++total;
    }
    end = clock();
    std::cout << "single thread result: " << total << std::endl;
    std::cout << "single thread time: " << end - start << std::endl;
 
    total = 0;
    start = clock();
    for (int i = 0; i < thnum; ++i) {
        threads[i] = std::thread(thread_click);
    }
    for (int i = 0; i < thnum; ++i) {
        threads[i].join();
    }
    end = clock();
    std::cout << "multi thread no mutex result: " << total << std::endl;
    std::cout << "multi thread no mutex time: " << end - start << std::endl;
 
    total = 0;
    start = clock();
    for (int i = 0; i < thnum; ++i) {
        threads[i] = std::thread(atomic_click);
    }
    for (int i = 0; i < thnum; ++i) {
        threads[i].join();
    }
    end = clock();
    std::cout << "multi thread atomic result: " << total2 << std::endl;
    std::cout << "multi thread atomic time: " << end - start << std::endl;
 
    total = 0;
    start = clock();
    for (int i = 0; i < thnum; ++i) {
        threads[i] = std::thread(mutex_click);
    }
    for (int i = 0; i < thnum; ++i) {
        threads[i].join();
    }
    end = clock();
    std::cout << "multi thread mutex result: " << total << std::endl;
    std::cout << "multi thread mutex time: " << end - start << std::endl;
 
    total = 0;
    start = clock();
    for (int i = 0; i < thnum; ++i) {
        threads[i] = std::thread(spinlock_click);
    }
    for (int i = 0; i < thnum; ++i) {
        threads[i].join();
    }
    end = clock();
    std::cout << "spin lock result: " << total << std::endl;
    std::cout << "spin lock time: " << end - start << std::endl;
    getchar();
    return 0;
}
 
/*
single thread result: 100000000
single thread time: 231
multi thread no mutex result: 11501106
multi thread no mutex time: 261
multi thread atomic result: 100000000
multi thread atomic time: 1882
multi thread mutex result: 100000000
multi thread mutex time: 16882
spin lock result: 100000000
spin lock time: 45063
*/
