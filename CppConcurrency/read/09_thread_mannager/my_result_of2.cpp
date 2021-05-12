/*
 * @Author: your name
 * @Date: 2021-03-08 14:24:31
 * @LastEditTime: 2021-03-08 14:43:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/09_thread_mannager/my_result_of2.cpp
 * 
 * https://www.geek-share.com/detail/2713711145.html
 */
#include<iostream>  
#include<map>  
#include<string>  
#include<vector>  
#include<algorithm>  
#include <utility>  
using namespace std;  

struct Person  
{  
	string name;  
	int age;  
	string city;  
};  

vector<Person> vt ={  
	{"aa",20,"shanghai"},  
	{"bb",25,"beijing"},  
	{"cc",20,"nanjing"},  
	{"dd",25,"nanjing"}  
};  

//Group_by函数  
template<typename Fn>  
multimap<typename result_of<Fn(Person)>::type, Person> GroupBy(const vector<Person>& vt,const Fn& keySelect)  
{  
	typedef typename result_of<Fn(Person)>::type key_type;  
	multimap<key_type,Person> map;  
	for_each(vt.begin(),vt.end(),  
			[&](const Person& p){  
			map.insert(make_pair(keySelect(p),p));  
			});  
	return map;  
}  

int main()  
{  
	//按年龄分组  
	auto res = GroupBy(vt, [](const Person& p){ return p.age; });  
	//按城市分组  
	auto res1 = GroupBy(vt, [](const Person& p) { return p.city; });  

	//打印结果  
	cout << "----------group by age:---------------" << endl;  
	for_each(res.begin(),res.end(),[](decltype(res)::value_type & p){  
			cout << p.second.name <<" " << p.second.city << "  " << p.second.age  << endl;  
			});  
	cout << "----------group by city:---------------"<< endl;  
	for_each(res1.begin(),res1.end(),[](decltype(res1)::value_type & p){  
			cout << p.second.name <<" " << p.second.city << "  " << p.second.age  << endl;  
			});  
	return 0;  
} 
