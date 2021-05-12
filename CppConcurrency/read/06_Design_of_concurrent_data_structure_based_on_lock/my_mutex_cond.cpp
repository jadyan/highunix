/*
 * @Author: your name
 * @Date: 2021-02-25 09:28:56
 * @LastEditTime: 2021-02-25 13:45:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/06_Design_of_concurrent_data_structure_based_on_lock/my_mutex.cpp
 */
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
class safeData{

private:
    std::mutex mut;
    std::condition_variable  cond;
    vector<int> vec;

public:
    safeData(){};
    void getDate()
    {
        cout<<"获取...."<<endl;
        //std::lock_guard<std::mutex> lk(mut);
        std::unique_lock<std::mutex> lk(mut);
        cond.wait(lk, [this] { return (this->vec.size() != 0);});
        cout<<"xxx"<<endl;
        for (int i=0;i<vec.size();i++)
        {
            cout<<"获取 = "<<vec[i] << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        vec.clear();
    }

    void setDate()
    {
        cout<<"设置...."<<endl;
        std::lock_guard<std::mutex> lk(mut);
        //std::unique_lock<std::mutex> lk(mut);
        for(int i=1; i<10; i++)
        {
            std::cout<<"设置 = "<<i*100-2<<endl;
            vec.push_back(i*100-2); 
            mut.unlock();
            cond.notify_one();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
       // cout<<"设置结束...."<<endl;
    }

    static safeData& getinstace()
    {
        static  safeData sd;
        return  sd;
    }

};

void readfun()
{
    safeData::getinstace().getDate();

}

void setfun()
{
    safeData::getinstace().setDate();

}

int main()
{
    std::thread th1(readfun);
    std::thread th2(setfun);

    th1.join();
    th2.join();
    return 0;
}