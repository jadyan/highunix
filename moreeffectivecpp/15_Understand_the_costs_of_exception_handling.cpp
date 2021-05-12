/*
 * @Author: your name
 * @Date: 2021-03-31 15:45:52
 * @LastEditTime: 2021-03-31 15:49:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/15_Understand_the_costs_of_exception_handling.cpp
 */
#include <iostream>
using namespace std;



int main(int argc, char** argv)
{
    string str="使用任何异常处理特性也要付出的代价谈起。你需要空间建立数据结构\
来跟踪对象是否被完全构造（constructed）(参见条款 M10)，你也需要 CPU 时间保持这些 \
数据结构不断更新。这些开销一般不是很大，但是采用不支持异常的方法编译的程序一般比  \
支持异常的程序运行速度更快所占空间也更小。\n";


    cout<<"str"<<str;
}