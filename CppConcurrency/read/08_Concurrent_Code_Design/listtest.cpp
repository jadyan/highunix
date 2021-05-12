/*
 * @Author: your name
 * @Date: 2021-03-03 14:04:52
 * @LastEditTime: 2021-03-09 09:48:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/08_Concurrent_Code_Design/listtest.cpp
 * 
 * 
 * 
 * list::splice实现list拼接的功能。将源list的内容部分或全部元素删除，拼插入到目的list。

函数有以下三种声明：

一：void splice ( iterator position, list<T,Allocator>& x );

二：void splice ( iterator position, list<T,Allocator>& x, iterator it );

三：void splice ( iterator position, list<T,Allocator>& x, iterator first, iterator last );

解释：

position 是要操作的list对象的迭代器

list&x 被剪的对象

对于一：会在position后把list&x所有的元素到剪接到要操作的list对象
对于二：只会把it的值剪接到要操作的list对象中
对于三：把first 到 last 剪接到要操作的list对象中

 */



#include<bits/stdc++.h>
using namespace std;


bool IsOdd (int i) { //用语判断奇数
    return (i%2)==1; //奇数返回true，偶数返回0
	
}

int partitionfun(void)
{

    std::cout<<""<<std::endl;
    std::vector<int> ivec;  
    for (int i=1; i<10; ++i) 
        ivec.push_back(i); // 1 2 3 4 5 6 7 8 9 

    auto bound = std::partition (ivec.begin(), ivec.end(), IsOdd);   

    std::cout << "odd elements:"<<endl;

    for (auto it=ivec.begin(); it!=bound; ++it)
        std::cout << ' ' << *it;       //输出1,9,3,7,5  

    std::cout<<""<<std::endl;
    std::cout << "even elements:"<<endl;
    
    for (auto it=bound; it!=ivec.end(); ++it)
        std::cout << ' ' << *it;     //输出6,4,8,2    

    std::cout<<""<<std::endl;
    return 0;
}


int main()
{
	list<int>li1,li2;
	for(int i=1;i<=4;i++) li1.push_back(i),li2.push_back(i+10);
	// li1 1 2 3 4
	// li2 11 12 13 14
	list<int>::iterator it=li1.begin();
	it++;
	
	/*
	合并 在it的前面插入，li2
	*/
	li1.splice(it,li2);//1 11 12 13 14 2 3 4
	for(list<int>::iterator it=li1.begin();it!=li1.end();++it) cout<<*it<<"  ";
	cout<<endl;


	if(li2.empty()) cout<<"li2 is empty"<<endl;
	
	//只会把it的值剪接到要操作的list对象中
	li2.splice(li2.begin(),li1,it);
	for(list<int>::iterator it=li2.begin();it!=li2.end();++it) cout<<*it<<"  ";
	cout<<*it<<"   chen"<<endl;

	list<int>li3;

	li3.splice(li1.begin(),li1,it,li1.end());
	
#if 0
	/*
	li1 1 11 12 13 14 3 4
	li2 2
	这里的it的值还是2  但是指向的已经是li2中的了 
	*/
	
	it=li1.begin();
	advance(it,3);//advance 的意思是增加的意思，就是相当于 it=it+3;这里指向13
	std::cout<<"xxxx="<<std::endl;
	for(list<int>::iterator it=li1.begin();it!=li1.end();++it) cout<<*it<<"  ";
	li1.splice(li1.begin(),li1,it,li1.end()); //13 14 3 4 1 11 12 可以发现it到li1.end()被剪贴到li1.begin()前面了 
	for(list<int>::iterator it=li1.begin();it!=li1.end();++it) cout<<*it<<"  ";
	std::cout<<"xxxx="<<std::endl;


	for(list<int>::iterator it=li1.begin();it!=li1.end();++it) cout<<*it<<"  ";
	cout<<endl;
	for(list<int>::iterator it=li2.begin();it!=li2.end();++it) cout<<*it<<"  ";


    partitionfun();
	return 0;
#endif 
}