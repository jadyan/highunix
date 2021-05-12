/*
 * @Author: your name
 * @Date: 2021-02-26 09:05:18
 * @LastEditTime: 2021-02-26 16:02:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/07_Design_of_concurrent_data_structure_based_not_lock/my_lock_free_stack_all.cpp
 */
//无锁的线程安全栈
#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <chrono>



/*
https://www.yuque.com/barret/snelnn/gmdywx
多个线程pop时候，把要删除的节点链表的最后，计数--，
当只有一个线程删除的时候，把当前节点删除，同时循环删除最后的删除列表

栈每次pop的时候判断是否只有当前一个（threads_in_pop）
如果为1的话，只有一个pop操作 删除当前节点，循环删除标记为1的删除链表，如果此时又有线程来pop，重新把删除链表放回到删除列表
如果不为1的话，把当前node放到删除链表中

判断当前pop 是否在执行
*/

using  namespace std;

template <typename T>
class lock_free_stack
{
private:
    struct node
    {
        std::shared_ptr<T> data; //使用共享指针保存数据
        node *next;

        node(const T &data) : data(std::make_shared<T>(data)), next(nullptr) {}
    };

    std::atomic<node *> head; //原子操作类

    std::atomic<node *> to_be_deleted;    //将要删除的节点列表，暂存，因为不能立刻删除，其他线程可能还在用
    std::atomic<unsigned> threads_in_pop;  // 1 原子变量

        //释放node
    static void delete_nodes(node *nodes)
    {
        while (nodes)
        {
            node *next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void chain_pending_nodes(node *nodes)
    {
        node *last = nodes;
        while (node *const next = last->next) // 9 让next指针指向链表的末尾
        {
            last = next;
        }
        chain_pending_nodes(nodes, last);
    }

    void chain_pending_nodes(node *first, node *last)
    {
        if (first==nullptr||last == nullptr)
        {
            return ;
        }
        last->next = to_be_deleted;
        while (!to_be_deleted.compare_exchange_weak(last->next, first)) // 用循环来保证last->next的正确性
            ;
    }
    void chain_pending_node(node *n)
    {
        chain_pending_nodes(n, n);
    }

    void try_reclaim(node *old_head)
    {
        if (threads_in_pop == 1) // 当计数为1，表示只有当前线程正在调用pop函数，可以删除节点
        {
            // 返回to_be_deleted的第一个值值给左边，同时to_be_deleted赋值nullptr，因为to_be_deleted中可能有多个等待中的节点
            node *nodes_to_delete = to_be_deleted.exchange(nullptr);
            if (!--threads_in_pop) // 计数器减一，再次确认是否只有一个线程调用pop()
            {
                delete_nodes(nodes_to_delete); // 循环删除等待列表中的节点
            }
            //若果当前threads_in_pop不为1 把nodes_to_delete 重新放回到删除列表
            else if (nodes_to_delete)
            {
                chain_pending_nodes(nodes_to_delete); // 6
            }
            delete old_head; // 删除新传入的节点
        }
        else
        {
            chain_pending_node(old_head); // 计数不是1，将节点添加到等待删除列表
            --threads_in_pop;             //计数减一
        }
    }

public:
    /*
    push操作：
    1. 创建一个新的节点node
    2. 让node->next指向head->next
    3. head->next指向node
    */
    void push(T const &data)
    {
        node *const new_node = new node(data); // 1
        // 2 node->next先指向head，这里会有多线程等待，如果别的线程在操作head的话
        new_node->next = head.load(); //使用默认内存序
        /*
        3 使用“比较-交换”更新head：
        判断当前head是不是new_node->next(用来判断是否其他线程修改了head)：
            返回false，则当前head不是new_node->next，则更新new_node->next为新的head，然后继续循环直到返回true
            返回true，则表示没有其他线程修改head，则替换head和new_node位置（new_node变成head之后的第一个了），跳出循环
        */
        while (!head.compare_exchange_weak(new_node->next, new_node))
            ;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    /*
    pop操作：
    1. 获取head。
    2. 读取head->next指向的结点node。
    3. 设置head->next指向node->next。
    4. 通过node返回携带的数据data。
    5. 删除node节点。
    */
    std::shared_ptr<T> pop()
    {
        ++threads_in_pop;
        //threads_in_pop.store(1);
        node *old_head = head.load(); //先获得head，多线程等待，防止别的线程正在获取head
        /*
        使用“比较-交换”更新head：
        判断当前head是否等于old_head，即head有无发生变化
            返回false，即head发生变化，old_head更新为新的head，继续循环
            返回true，即head没有变化，则替换head和head->next的位置，跳出循环
        */
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next)) //old_head不能为空指针
            ;

        //return old_head ? old_head->data : std::shared_ptr<T>();//返回数据或空指针
        
        //此时的old_head为第一个有数据的节点，真正的head是它的next
        std::shared_ptr<T> retVal;
        if (old_head)
        {
            retVal.swap(old_head->data); //交换内容，将old_head的data传递给retVal
        }
        try_reclaim(old_head); //回收old_head节点，计数器减一
        return retVal;
        
    }

    static lock_free_stack& instance()
    {
        static lock_free_stack lfs;
        return lfs;
    }

};


void pushthread()
{

    for(int i=0;i<100;i++)
    {
        cout<<"push stack"<<i<<endl;
        lock_free_stack<int>::instance().push(i);
        // lock_free_stack<int> lfs;
        // lfs.push(i);
    }

}

void popthread()
{
    while (true)
    {
        std::shared_ptr<int> pi = lock_free_stack<int>::instance().pop();
        if(pi !=nullptr)
        {
            cout<<"pop stack"<<*pi<<endl;   
        }
        
    }
    
}


int main()
{
    std::thread th1(pushthread); 
    std::thread th3(pushthread); 
    std::thread th4(pushthread); 
    std::thread th9(pushthread);
    std::thread th10(pushthread);


    std::thread th2(popthread); 
    std::thread th5(popthread);
    std::thread th6(popthread);
    std::thread th7(popthread);
    std::thread th8(popthread);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();
    return 0;
}
