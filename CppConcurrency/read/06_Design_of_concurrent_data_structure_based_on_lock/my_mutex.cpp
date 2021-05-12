/*
 * @Author: your name
 * @Date: 2021-02-25 09:28:56
 * @LastEditTime: 2021-02-25 09:58:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/06_Design_of_concurrent_data_structure_based_on_lock/my_mutex.cpp
 */
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
class safeData{

private:
    std::mutex mut;
    vector<int> vec;

public:
    safeData(){};
    void getDate()
    {
        cout<<"获取...."<<endl;
        std::lock_guard<std::mutex> lk(mut);
        //cout<<"获取开始..."<<endl;
        for (int i=0;i<vec.size();i++)
        {
            cout<<"获取 = "<<vec[i] << endl;
            cout<<"获取开始..."<<endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void setDate()
    {
        cout<<"设置...."<<endl;
        std::lock_guard<std::mutex> lk(mut);
        for(int i=1; i<10; i++)
        {
            std::cout<<"设置 = "<<i*100-2<<endl;
            vec.push_back(i*100-2);
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