/*
 * @Author: your name
 * @Date: 2021-05-12 09:49:20
 * @LastEditTime: 2021-05-12 13:22:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/27_03_Requiring_or_prohibiting_heap_based_objects.cpp
 */
#include <iostream> 
#include <string> 
#include <vector> 
#include <list> 
#include <algorithm>

using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 
using std::list; 
using std::ostream; 

class HeapTracked
{
	public:
		virtual ~HeapTracked(){ }
		static void* operator new(size_t size); 
		static void operator delete(void *ptr); 

		bool isOnHeap() const; 

	private:
		static list<void*> address; 
}; 

list<void*> HeapTracked::address; 

void* HeapTracked::operator new(size_t size)
{
	void *p = ::operator new(size); 
	cout << "new " << p << endl; 
	address.push_front(p); 
	return p; 
}

void HeapTracked::operator delete(void *ptr)
{
	list<void*>::iterator it = find(
			address.begin(), address.end(), ptr); 
	if(it != address.end())
	{
		cout << "delete " << ptr << endl; 
		address.erase(it); 
		::operator delete(ptr); 
	}
	else 
		cout << "missing address " << ptr << endl; 
}

bool HeapTracked::isOnHeap() const
{
	const void *p = dynamic_cast<const void*>(this); 
	list<void*>::iterator it = find(
			address.begin(), address.end(), p); 
	return it != address.end(); 
}

class UPNumber : public HeapTracked
{
	public:
		UPNumber() 
        { 
            cout << "UPNumber 构造函数" << endl; 
        }
		void destroy() 
        { 
            if(isOnHeap()) 
                delete this; 
        } 
		~UPNumber() 
        {  
            cout << "UPNumber 析构函数" << endl; 
        } 
}; 

class NegativeUPNumber : public UPNumber
{
}; 

int main()
{
	UPNumber n; 
	UPNumber *p = new UPNumber(); 
	n.destroy(); 
	p->destroy(); 

cout<<"....子类...."<<endl;
	NegativeUPNumber n1; 
	NegativeUPNumber *n2 = new NegativeUPNumber; 
	n1.destroy(); 
	n2->destroy(); 
	return 0; 
}
