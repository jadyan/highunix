/*
 * @Author: jadyan
 * @Date: 2021-01-22 17:49:14
 * @LastEditTime: 2021-01-27 09:19:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/2.5.cpp
 */


/*
void some_function();
void some_other_function();
std::thread t1(some_function);            // 1
std::thread t2=std::move(t1);            // 2
t1=std::thread(some_other_function);    // 3
std::thread t3;                            // 4
t3=std::move(t2);                        // 5
t1=std::move(t3);                        // 6 赋值操作将使程序崩溃

最后一个移动操作，将some_function线程的所有权转移⑥给t1。
不过，t1已经有了一个关联的线程(执行some_other_function的线程)，
所以这里系统直接调用std::terminate()终止程序继续运行。
这样做（不抛出异常，std::terminate()是noexcept函数)是为了保证与std::thread的析构函数的行为一致。2.1.1节中，需要在线程对象被析构前，显式的等待线程完成，或者分离它；进行赋值时也需要满足这些条件(说明：不能通过赋一个新值给std::thread对象的方式来”丢弃”一个线程)。
*/

#include <iostream>
#include <thread>
#include <string>

using namespace std;
void some_function() {
    cout<<"some_function 线程开始!"<<endl;
}

void some_other_function(int i )  {
    cout<<"some_other_function 线程开始! i = "<<i<<endl;
}



/*
当所有权可以在函数内部传递，就允许std::thread实例可作为参数进行传递
*/
std::thread f() {
    void some_function();
    return std::thread(some_function);
}

std::thread g() {
    void some_other_function(int);
    std::thread t(some_other_function, 42);
    return t;
}

int main() {
    std::thread t1 = f();
    t1.join();
    std::thread t2 = g();
    t2.join();
}
