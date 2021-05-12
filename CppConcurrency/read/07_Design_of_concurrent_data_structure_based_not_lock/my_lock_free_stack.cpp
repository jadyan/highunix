#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <unistd.h>
#include <atomic>
#include <memory>
using namespace std;



/*
新建一个节点
将该节点的next指针指向现有栈顶
更新栈顶
codenong.com/cs107035480/
https://blog.csdn.net/yand789/article/details/27324295
*/
template<typename T>
class lock_free_stack//栈的底层数据结构采用单向链表实现
{
private:
    struct node
    {
        std::shared_ptr<T> data;//这里采用shared_ptr管理的好处在于：若栈内存放对象pop中return栈顶对象可能拷贝异常，栈内只存储指针还可以提高性能
        node* next;
        node(T const& data_):data(std::make_shared<T>(data_)) , next(nullptr)//注意make_shared比直接shared_ptr构造的内存开销小
        {}
    };
    std::atomic<node*> head;//采用原子类型管理栈顶元素，主要利用atomic::compare_exchange_weak实现lock free
public:
    void push(T const& data)
    {
        node* const new_node=new node(data);
        new_node->next=head.load();//每次从链表头插入

        /*
            若head==new_node->next则更新head为new_node,返回true结束循环，插入成功; 
            若head!=new_node->next表明有其它线程在此期间对head操作了，将new_node->next更新为新的head，返回false，
            继续进入下一次while循环。
            这里采用atomic::compare_exchange_weak比atomic::compare_exchange_strong快，
            因为compare_exchange_weak可能在元素相等的时候返回false所以适合在循环中，
            而atomic::compare_exchange_strong保证了比较的正确性，不适合用于循环
        */

        while(!head.compare_exchange_weak(new_node->next,new_node));
    }
  /*  std::shared_ptr<T> pop()
    {
        node* old_head=head.load();//拿住栈顶元素，但是可能后续被更新，更新发生在head!=old_head时
        while(old_head &&!head.compare_exchange_weak(old_head,old_head->next));//这里注意首先要先判断old_head是否为nullptr防止操作空链表，然后按照compare_exchange_weak语义更新链表头结点。若head==old_head则更新head为old_head->next并返回true，结束循环，删除栈顶元素成功;若head!=old_head表明在此期间有其它线程操作了head，因此更新old_head为新的head,返回false进入下一轮循环，直至删除成功。
        return old_head ? old_head->data : std::shared_ptr<T>();//这里注意空链表时返回的是一个空的shared_ptr对象
    }//这里只是lock free，由于while循环可能无限期循环不能在有限步骤内完成，故不是wait free
   
    */
    std::shared_ptr<T> pop()//T pop()
    {
        while(1){
            auto result = head.load(std::memory_order_relaxed);
            if (result == nullptr)
            {     //continue;//throw std::string("Cannot pop from empty stack");
                return nullptr; 
            }

            /*
            compare_exchange_weak( T& expected, T desired, std::memory_order success, std::memory_order failure );
            success 对应于read-modify-write的内存模型；failure则对应于失败时的load
            */
            if(head.compare_exchange_weak(result,result->next,
                                          std::memory_order_release,
                                          std::memory_order_relaxed))
            {
               return result->data;
            }

        }
    }
};

std::atomic<bool> ready(false);
std::atomic<bool> winner(false);
lock_free_stack<int> lfs;
void count1m (int id)
{
    while (!ready) {}                  // wait for the ready signal
    for (int i = 0; i < 1000000; ++i) {}   // go!, count to 1 million
    if (!winner.exchange(true)) { std::cout << "thread #" << id << " won!\n"; }
};

void putdata()
{
 
   int value=1;
   for(int i=0;i<1000;i++)
   {
       value=i;
       lfs.push(value);
       usleep(1);
    }
}


void popdata()
{
 
   shared_ptr<int> value=nullptr;
  
    for(int i=0;i<1000;i++)
   {
       value=lfs.pop();
       if(value!=nullptr)
        {
           // std::cout<<"i:"<<i<<" value"<<*value<<std::endl;
        }else
        {
          cout<<"nullptr"<<"i:"<<i<<endl;
         
        }
    } 
  
 
}

int main ()
{
 /*
    std::vector<std::thread> threads;
    std::cout << "spawning 10 threads that count to 1 million...\n";
    for (int i = 1; i <= 10; ++i) threads.push_back(std::thread(count1m,i));
    ready = true;
    for (auto& th : threads) th.join();
*/

 
  
   thread  thread_put=thread(putdata);

   thread  thread_put2=thread(putdata);

   thread  thread_pop=thread(popdata);
  
   thread_put.join();
   thread_put2.join();
   thread_pop.join();
  
    return 0;
}