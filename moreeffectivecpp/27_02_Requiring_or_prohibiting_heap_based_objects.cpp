/*
 * @Author: your name
 * @Date: 2021-05-07 10:43:20
 * @LastEditTime: 2021-05-11 09:32:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/27_02_Requiring_or_prohibiting_heap_based_objects.cpp
 */

#include <iostream>

using namespace std;

class UPNumber 
{
public:
	// 如果建立一个非堆对象，抛出一个异常
	class HeapConstraintViolation {};
	static void * operator new(size_t size);
	UPNumber();

    void *  operator new[](size_t size)
    {
        cout<<"operator new[] 函数"<<size<<endl;
	    onTheHeap = true;
	    return ::operator new [] (size);
    }

private:
	static bool onTheHeap; //在构造函数内，指示对象是否被构造在堆上
    long b;
};
// obligatory definition of class static
bool UPNumber::onTheHeap = false;

//--注意这里：当调用了new操作，则认为是在堆中
void *UPNumber::operator new(size_t size)
{
    cout<<"operator new 函数"<<size<<endl;
	onTheHeap = true;
	return ::operator new(size);
}



UPNumber::UPNumber()
{
    cout<<"\tUPNumber构造"<<endl;
    try{
        if (!onTheHeap) {
		    throw HeapConstraintViolation();
        }
    }catch(HeapConstraintViolation & e){
        cout<<"\t只能在堆上分配(只能通过new来实例化对象)"<<endl;
    }
	onTheHeap = false; // 为下一个对象清除标记
}


int main(int argc, char ** argv)
{
    UPNumber *pu =  new UPNumber();
    //UPNumber *p = (UPNumber*)operator new(sizeof(UPNumber));
    UPNumber Up2;



    cout<<"------------------new 出来数组:---------------------"<<endl;


    /*
    会存在两个问题： 第一，这里调用的operator new[] 操作，所以onTheHeap不会为真，可以重载new []操作来解决；
     第二：这里会调用100次构造函数，但是只在最开始的时候分配了一次内存，所以只有第一次调用构造函数前把onTheHeap设置为true。当调用第二个构造函数时，会抛出一个异常。
    */
    UPNumber *numberArray = new UPNumber[3];
    return 0;
}