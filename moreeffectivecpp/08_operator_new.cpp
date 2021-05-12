/*
 * @Author: your name
 * @Date: 2021-03-23 20:55:15
 * @LastEditTime: 2021-03-23 21:03:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/08_operator_new.cpp
 */
#include <iostream>
#include <memory>
using namespace std;

class Widget {
public:
        Widget(int widget8Size) 
        {
            cout<<"Widget 构造函数"<<endl;
        }
};

void* mallocShared(size_t size)
{
	return operator new(size);
}

void freeShared(void* memory)
{
	operator delete(memory);
}

Widget* constructWidget8InBuffer(void* buffer, int widget8Size)
{
	return new(buffer) Widget(widget8Size); // new操作符的一个用法，需要使用一个额外的变量(buffer)，当new操作符隐含调用operator new函数时，把这个变量传递给它
	// 被调用的operator new函数除了待有强制的参数size_t外，还必须接受void*指针参数，指向构造对象占用的内存空间。这个operator new就是placement new,它看上去像这样:
	// void * operator new(size_t, void* location) { return location; }
}

int test_item_8()
{
	std::string* ps = new std::string("Memory Management"); // 使用的new是new操作符(new operator)

	//void * operator new(size_t size); // 函数operator new通常声明
	void* rawMemory = operator new(sizeof(std::string)); // 操作符operator new将返回一个指针，指向一块足够容纳一个string类型对象的内存
	operator delete(rawMemory);

	delete ps; // ps->~std::string(); operator delete(ps);

	void* buffer = operator new(50*sizeof(char)); // 分配足够的内存以容纳50个char，没有调用构造函数
	operator delete(buffer); // 释放内存，没有调用析构函数. 这与在C中调用malloc和free等同OA

	void* sharedMemory = mallocShared(sizeof(Widget));

	Widget* pw = constructWidget8InBuffer(sharedMemory, 10); // placement new

	//delete pw; // 结果不确定，共享内存来自mallocShared,而不是operator new
	pw->~Widget(); // 正确，析构pw指向的Widget8,但是没有释放包含Widget8的内存
	freeShared(pw); // 正确，释放pw指向的共享内存，但是没有调用析构函数
	return 0;
}


int main(int argc, char ** argv)
{
    test_item_8();
    return 0;
}