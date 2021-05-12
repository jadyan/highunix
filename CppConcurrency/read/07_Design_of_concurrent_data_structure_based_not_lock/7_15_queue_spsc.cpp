/*
单线程版本
*/
#include <iostream>
#include <cstdio>
#include <atomic>
#include <memory>
#include <thread>

template <typename T>
class queue
{
  private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;

        node()
        : next(nullptr)
        {}
    };

    std::atomic<node*> head;
    std::atomic<node*> tail;
    
    node* pop_head()
    {
        node* const old_head = head.load();
        if (old_head == tail.load()) {
            return nullptr;
        }
        head.store(old_head->next);
        return old_head;
    }
  public:
    queue()
    : head(new node)
    , tail(head.load())
    {}

    queue(const queue& other)=delete;
    queue& operator=(const queue& other)=delete;

    ~queue()
    {
        while (node* const old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    std::shared_ptr<T> pop()
    {
        node* old_head = pop_head();
        if (!old_head) {
            return std::shared_ptr<T>();
        }

        std::shared_ptr<T> const res(old_head->data);
        if(old_head != nullptr){
            delete old_head;
            old_head=nullptr;
        }

        return res;
    }

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
        node* p = new node;
        node* const old_tail = tail.load();
        old_tail->data.swap(new_data);
        old_tail->next = p;
        tail.store(p);
    }
};



queue<int> q;

void pushthread()
{
    for (int i = 0; i < 100; ++i) {
        printf("pushing %d\n", i);
        q.push(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void popthread()
{
    int i = 0;
    while (1) {
        std::shared_ptr<int> p = q.pop();
        if (p) {
            printf("poping %d\n", *p);
            ++i;
        }
    }
}

int main()
{

    std::thread th1(pushthread); 
    // std::thread th3(pushthread); 
    // std::thread th4(pushthread); 
    // std::thread th9(pushthread);
    // std::thread th10(pushthread);


    std::thread th2(popthread); 
    // std::thread th5(popthread);
    // std::thread th6(popthread);
    // std::thread th7(popthread);
    // std::thread th8(popthread);

    th1.join();
    th2.join();
    // th3.join();
    // th4.join();
    // th5.join();
    // th6.join();
    // th7.join();
    // th8.join();
    // th9.join();
    // th10.join();
    return 0;
}