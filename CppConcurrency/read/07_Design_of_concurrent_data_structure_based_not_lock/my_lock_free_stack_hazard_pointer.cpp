#include <iostream>
#include <cstdio>
#include <atomic>
#include <memory>
#include <thread>
#include <chrono>


/*
风险指针： 把当前要出栈的node标记为风险指针， 删除时候，判断是否有没有其他线程也标记为了风险指针，把该节点加入到删除链表，
        如果其他线程没有标记为风险指针，直接删除，然后，在循环删除一下风险链表，如果还在风险链表里，则再加到风险链表里
判断当前节点是否在用
*/

unsigned const max_hazard_pointers = 100;
struct hazard_pointer
{
    std::atomic<std::thread::id> id;
    std::atomic<void*> pointer;
};

hazard_pointer hazard_pointers[max_hazard_pointers];

class hp_owner
{
    hazard_pointer* hp;

  public:
    hp_owner(hp_owner const&)=delete;
    hp_owner operator=(hp_owner const&)=delete;
    hp_owner(): hp(nullptr)
    {
        for (unsigned i = 0; i < max_hazard_pointers; ++i) {
            std::thread::id old_id;
            // try to claim ownership of a hazard pointer
            // 设置claim ownership 指针所有者
#if 0
                std::cout<<"____"<<hazard_pointers[i].id<<std::endl;
                std::cout<<"____"<<old_id<<std::endl;
                std::cout<<"____"<<std::this_thread::get_id()<<std::endl;
#endif
            /*
            未赋值之前old_id 与 hazard_pointers[i].id 的值都是<thread::id of a non-executing thread>
            */
            if (hazard_pointers[i].id.compare_exchange_strong(old_id, std::this_thread::get_id())) 
            {

                // successfully claimed the entry for the current thread,
                // store it and stop the search
                hp = &hazard_pointers[i];
                printf("i, hp: %ld\n", i, hp);
                break;
            }
            // another threads owns this entry, move on to the next
        }
        // if you get to the end of the list without finding a free entry,
        // there are too many threads using hazard pointers, so throw an
        // exception
        if (!hp) {
            throw std::runtime_error("No hazard pointers available");
        }
    }

    std::atomic<void*>& get_pointer()
    {
        return hp->pointer;
    }

    ~hp_owner()
    {
        // when each thread exits, if an instance of hp_owner was created
        // for the thread, then it's destryoed. The destructor then resets
        // the actual pointer to nullptr before setting the owner ID to 
        // std::thread::id(), allowing another thread to reuse the entry later.
        // TODO: this causes crash
        //hp->pointer.store(nullptr);
        //hp->id.store(std::thread::id());
    }
};




/*
风险指针。指针必须能让所有线程看到，需要线程能够对数据结构进行访问
返回风险指针的引用。当读取一个指针，并且想要解引用它的时候，就需要这个函数
*/
std::atomic<void*>& get_hazard_pointer_for_current_thread()
{
    // The first time each thread calls this function, a new instance of
    // hp_owner is created. The constructor for this new instance then
    // searchs through the table of owner/pointer pairs looking for an entry
    // without an owner. It uses compare_exchange_strong() to check for an 
    // entry without an owner and claim it in one go.
    //
    // Once the hp_owner instance has been created for a given thread, further
    // accesses are much faster because the pointer is cached, so the table
    // doesn't have to be scanned again.
    
    //printf("get harzard pointer for current thread, thread id: %d\n", std::this_thread::get_id());
    //printf("当前线程获取风险指针, thread id: %d\n", std::this_thread::get_id());
    //4 每个线程都有自己的风险指针
    thread_local static hp_owner hazard;
    //5 返回这个变量所持有的指针 返回当前线程的风险指针
    return hazard.get_pointer();
}

/*
判断风险指针是否在其他线程被标记
*/
bool outstanding_hazard_pointers_for(void *p)
{
    for (unsigned i = 0; i < max_hazard_pointers; ++i) {
        if (hazard_pointers[i].pointer.load() == p) {
            return true;
        }
    }
    return false;
}

template <typename T>
void do_delete(void* p)
{
    delete static_cast<T*>(p);
}

struct data_to_reclaim
{
    void* data;
    std::function<void(void *)> deleter;
    data_to_reclaim* next;

    template<typename T>
    data_to_reclaim(T* p)
    : data(p)
    , deleter(&do_delete<T>)
    , next(0)
    {}

    ~data_to_reclaim()
    {
        deleter(data);
    }
};

std::atomic<data_to_reclaim*> nodes_to_reclaim;
void add_to_reclaim_list(data_to_reclaim* node)
{
    node->next = nodes_to_reclaim.load();
    while (!nodes_to_reclaim.compare_exchange_weak(node->next, node));
}

template<typename T>
void reclaim_later(T* data)
{
    add_to_reclaim_list(new data_to_reclaim(data));
}

void delete_nodes_with_no_hazards()
{
    // first claims the entire list of nodes to be reclaimed;
    // ensures that this is the only thread trying to reclaim
    // this particular set of nodes; other threads are now free
    // to add futher nodes to the list or event try to reclaim 
    // them without impacting the operation of this thread.
    data_to_reclaim* current = nodes_to_reclaim.exchange(nullptr);

    while (current) {
        data_to_reclaim* const next = current->next;

        // check each node in turn to see if there are any outstanding
        // hazard pointers.
        if (!outstanding_hazard_pointers_for(current->data)) {
            // if there aren't, delete the entry
            delete current;
        }
        else {
            // otherwise, just add the item back on the list for 
            // reclaiming later
            add_to_reclaim_list(current);
        }
        current=next;
    }
}

template <typename T>
class stack
{
  private:
    struct node
    {
        std::shared_ptr<T> data;
        node *next;

        node(const T& data_) 
        : data(std::make_shared<T>(data_))
        {}
    };

    std::atomic<node *> head;

  public:
    stack()
    : head(nullptr)
    {}

    void push(const T& data)
    {
        node *const new_node = new node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_weak(new_node->next, new_node));
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    std::shared_ptr<T> pop()
    {
        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
        node* old_head = head.load();
        do {
            node* temp;
            do { 
                // loop until you've set the harzard pointer to head
                //循环能保证node不会在读取旧head指针
                temp = old_head;
                hp.store(old_head);
                old_head = head.load();
            } while (old_head != temp);
        }
        while (old_head && 
               !head.compare_exchange_strong(old_head, old_head->next));
        hp.store(nullptr); // clear hazard pointer once you're finished
        std::shared_ptr<T> res;
        if (old_head) {
            res.swap(old_head->data);
            if (outstanding_hazard_pointers_for(old_head)) { 
                // check for hazard pointers referencing
                // a node before you delete it
                //加入到要删除的风险指针链表
                reclaim_later(old_head);
            }
            else {
                delete old_head;
            }
            delete_nodes_with_no_hazards();
        }
        return res;
    }
};

void push(stack<int>* s)
{
    printf("starting push\n");
    for (int i = 0; i < 10; ++i) {
        printf("pushing %d\n", i);
        s->push(i);
    }
}

void pop(stack<int>* s)
{
    printf("starting pop\n");
    int count = 0;
    std::shared_ptr<int> e;
    while (count < 10) {
        if (e = s->pop()) {
            printf("popping %d\n", *e);
            ++count;
        }
    }
}

int main()
{
    printf("creating stack\n");
    stack<int> s;
    printf("stack created\n");
    std::thread t1(push, &s);
    std::thread t11(push, &s);
    std::thread t12(push, &s);
    std::thread t13(push, &s);
    std::thread t14(push, &s);

    std::thread t2(pop, &s);
    std::thread t21(pop, &s);
    std::thread t22(pop, &s);
    std::thread t23(pop, &s);
    std::thread t24(pop, &s);



    t1.join();
    t2.join();

    t11.join();
    t12.join();
    t13.join();
    t14.join();

    t21.join();
    t22.join();
    t23.join();
    t24.join();
    return 0;
}