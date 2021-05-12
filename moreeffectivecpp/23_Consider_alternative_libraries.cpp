/*
 * @Author: your name
 * @Date: 2021-04-02 13:39:44
 * @LastEditTime: 2021-04-02 13:59:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/23_Consider_alternative_libraries.cpp
 */


/*
但是操作符<<既是类型安全（type-safe）又可
以扩展，而 printf 则不具有这两种优点

程序有 I/O 瓶颈，你可以考虑用 stdio 替代 iostream，如果程序在动态分配和释放内
存上使用了大量时间，你可以想想是否有其他的 operator new 和 operator delete 的实现
可用（参见条款 M8 和 Effective C++条款 10）。因为不同的程序库在效率、可扩展性、移植
性、类型安全和其他一些领域上蕴含着不同的设计理念，通过变换使用给予性能更多考虑的
程序库，你有时可以大幅度地提高软件的效率
*/

#define  STDIO

#ifdef STDIO
#include <stdio.h>
#else
#include <iostream>
#include <iomanip>
using namespace std;
#endif
const int VALUES = 30000; // # of values to read/write
int main()
{
    double d;
    for (int n = 1; n <= VALUES; ++n) {
#ifdef STDIO
        scanf("%lf", &d);
        printf("%10.5f", d);
#else
        cin >> d;
        cout << setw(10) // 设定 field 宽度
        << setprecision(5) // 设置小数位置
        << setiosflags(ios::showpoint) // keep trailing 0s
        << setiosflags(ios::fixed) // 使用这些设置
        << d;
#endif
        if (n % 5 == 0) {
#ifdef STDIO
            printf("\n");
#else
            cout << '\n';
#endif
        }
    }
return 0;
}