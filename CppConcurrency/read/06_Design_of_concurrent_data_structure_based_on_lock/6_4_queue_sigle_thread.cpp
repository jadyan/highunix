/*
 * @Author: your name
 * @Date: 2021-02-24 10:50:49
 * @LastEditTime: 2021-02-24 11:07:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/06_Design_of_concurrent_data_structure_based_on_lock/6_4_queue_sigle_thread.cpp
 */
//
//  6.4.cpp 基于锁的并发数据结构->线程安全队列一一使用细粒度锁和条件变量
//  Cpp-Concurrency
//
//  队列实现一一单线程版

#include <iostream>
#include <memory>

template <typename T>
class queue {
private:
    struct node {
        T data;
        std::unique_ptr<node> next;
        
        node(T data_): data(std::move(data_)) {}
    };
    
    std::unique_ptr<node> head;
    node* tail;
    
public:
    queue(): tail(nullptr) {}
    queue(const queue& other) = delete;
    queue& operator=(const queue& other) = delete;
    
    std::shared_ptr<T> try_pop() {
        if (!head) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(std::make_shared<T>(std::move(head->data)));
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }
    
    void push(T new_value) {
        std::unique_ptr<node> p(new node(std::move(new_value)));
        node* const new_tail = p.get();
        if (tail) {
            tail->next = std::move(p);
        }
        else {
            head = std::move(p);
        }
        tail = new_tail;
    }
};



int main() {
    queue<int> si;
    si.push(5);
    si.push(6);
    std::shared_ptr<int> p= si.try_pop();
    std::cout<<"p="<<*p<<std::endl;
    std::shared_ptr<int> p2= si.try_pop();
    std::cout<<"p2="<<*p2<<std::endl;
}
