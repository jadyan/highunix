/*
 * @Author: jadyan
 * @Date: 2021-01-22 17:48:08
 * @LastEditTime: 2021-01-26 17:15:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/2.4.cpp
 */

#include <iostream>

#include <thread>
#include <string>
#include <unistd.h>

void open_document_and_display_gui(std::string const& filename) {
    
}

bool done_editing() {
    //return true;
    static int  conncount=0;
    if (conncount<10)
    {
        conncount++;
        return false;
    }
    return true;
    
}

enum command_type {
    open_new_document
};

struct user_command {
    command_type type;
    
    user_command(): type(open_new_document) {}
};

user_command get_user_input() {
    return user_command();
}

std::string get_filename_from_user() {
    return "foo.doc";
}

void process_user_input(user_command const& cmd) {

}


/*
试想如何能让一个文字处理应用同时编辑多个文档。
无论是用户界面，还是在内部应用内部进行，都有很多的解决方法。
虽然，这些窗口看起来是完全独立的，每个窗口都有自己独立的菜单选项，
但他们却运行在同一个应用实例中。一种内部处理方式是，让每个文档处理窗口拥有自己的线程；
每个线程运行同样的的代码，并隔离不同窗口处理的数据。如此这般，打开一个文档就要启动一个新线程。
因为是对独立的文档进行操作，所以没有必要等待其他线程完成。因此，这里就可以让文档处理窗口运行在分离的线程上。



如果用户选择打开一个新文档，需要启动一个新线程去打开新文档①，
并分离线程②。与当前线程做出的操作一样，新线程只不过是打开另一个文件而已。
所以，edit_document函数可以复用，通过传参的形式打开新的文件。

这个例子也展示了传参启动线程的方法：
不仅可以向std::thread构造函数①传递函数名，还可以传递函数所需的参数(实参)
。C++线程库的方式也不是很复杂。
当然，也有其他方法完成这项功能，比如:使用一个带有数据成员的成员函数，代替一个需要传参的普通函数。
*/

void edit_document(std::string const& filename) {
    open_document_and_display_gui(filename);
    while (!done_editing()) {
        user_command cmd = get_user_input();
        if (cmd.type == open_new_document) {
            std::string const new_name = get_filename_from_user();
            // 不仅可以向thread构造函数传递函数名，还可以传递函数参数（实参）->
            std::thread t(edit_document, new_name); // -> 启动新线程
            t.detach(); // 分离线程
        }
        else {
            process_user_input(cmd);
        }
    }
}

int main() {
    edit_document("bar.doc");
    for (;;)
    {
        sleep(1);
    }
}
