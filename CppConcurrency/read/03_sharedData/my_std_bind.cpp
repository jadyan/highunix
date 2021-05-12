#include <iostream>
#include <functional>   //std::bind
using namespace std;

void func(int x, int y)
{
    cout << x << " " << y << endl;
}

int main()
{
    bind(func, 1, 2)();                     //输出：1 2
    bind(func, std::placeholders::_1, 2)(1);//输出：1 2

    using namespace std::placeholders;    // adds visibility of _1, _2, _3,...
    bind(func, 2, _1)(1);       //输出：2 1
    bind(func, 2, _2)(1, 2);    //输出：2 2
    bind(func, _1, _2)(1, 2);   //输出：1 2
    bind(func,_2, _1)(1, 2);    //输出：2 1

    //err, 调用时没有第二个参数
    //bind(func, 2, _2)(1);

    return 0;
}

