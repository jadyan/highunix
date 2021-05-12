/*
 * @Author: your name
 * @Date: 2021-02-01 10:51:16
 * @LastEditTime: 2021-02-01 10:55:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/my_unique.cpp
 * https://my.oschina.net/u/4391341/blog/3612035
 */
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock, std::unique_lock
                          // std::adopt_lock, std::defer_lock
std::mutex foo,bar,mtx;

void task_a () {
  std::lock (foo,bar);         // simultaneous lock (prevents deadlock)
  std::unique_lock<std::mutex> lck1 (foo,std::adopt_lock);
  std::unique_lock<std::mutex> lck2 (bar,std::adopt_lock);
  std::cout << "task a\n";
  // (unlocked automatically on destruction of lck1 and lck2)
}

void task_b () {
  // foo.lock(); bar.lock(); // replaced by:
  std::unique_lock<std::mutex> lck1, lck2;
  lck1 = std::unique_lock<std::mutex>(bar,std::defer_lock);
  lck2 = std::unique_lock<std::mutex>(foo,std::defer_lock);
  std::lock (lck1,lck2);       // simultaneous lock (prevents deadlock)
  std::cout << "task b\n";
  // (unlocked automatically on destruction of lck1 and lck2)
}



void print_fifty (char c) {
  //std::mutex mtx;
  std::unique_lock<std::mutex> lck;         // default-constructed
  lck = std::unique_lock<std::mutex>(mtx);  // move-assigned
  for (int i=0; i<50; ++i) { std::cout << c; }
  std::cout << '\n';
}

int main ()
{
  std::thread th1 (task_a);
  std::thread th2 (task_b);
  std::thread th3 (print_fifty,'*');

  th1.join();
  th2.join();
  th3.join();

  return 0;
}
