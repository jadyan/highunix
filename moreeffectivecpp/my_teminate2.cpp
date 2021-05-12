/*
 * @Author: your name
 * @Date: 2021-03-30 22:18:01
 * @LastEditTime: 2021-03-30 22:24:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/my_teminate2.cpp
 * 
 * https://segmentfault.com/a/1190000017791249
 */
#include <iostream>

using namespace std;

void my_unexpected()
{
    cout << "void my_unexpected()" << endl;
    
    // exit(1);
    
    throw 1;
}

void func() throw(int)
{
    cout << "func()" << endl;
    
    throw 250;
}

int main()
{
    set_unexpected(my_unexpected);

    try
    {
        func();
    }
    catch(int)
    {
        cout << "catch(int)" << endl;
    }
    catch(char)
    {
        cout << "catch(char)" << endl;
    }
    
    return 0;
}