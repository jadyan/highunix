/*
 * @Author: your name
 * @Date: 2021-01-27 10:47:17
 * @LastEditTime: 2021-01-27 10:58:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/my_std_bind2.cpp
 */
#include <iostream>
#include <functional>   //std::cout
using namespace std;
using namespace std::placeholders;    // adds visibility of _1, _2, _3,...


/*
https://blog.csdn.net/tennysonsky/article/details/77447804
*/
class Test
{
public:
    int i = 0;

    void func(int x, int y)
    {
        cout << x << " " << y << endl;
    }
};

int main()
{
    Test obj; //创建对象

    function<void(int, int)> f1 = bind(&Test::func, &obj, _1, _2);
    f1(1, 2);   //输出：1 2

    function< int &()> f2 = bind(&Test::i, &obj);
    f2() = 123;
    cout << obj.i << endl;//结果为 123

    return 0;
}
