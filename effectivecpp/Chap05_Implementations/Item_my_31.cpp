//Person.h  
//the handle class  
//定义  
#include <iostream>
#include <string>  
#include <memory> 
  
  /*
  https://blog.csdn.net/plutus_sutulp/article/details/9410839
  */
//class PersonImpl;  


class PersonImpl  
{  
public:  
    PersonImpl(const std::string& name,const int &id,const std::string& addr);  
    std::string rename() const;  
    int reid() const;  
    std::string readdr() const;  
private:  
    std::string theName;  
    int theId;  
    std::string theAddress;  
      
};  

class Person  
{  
public:  
    Person(const std::string& name,const int &id,const std::string& addr);  
    std::string rename() const;  
    int reid() const;  
    std::string readdr() const;  
private:  
    std::shared_ptr<PersonImpl> pImpl; //指针，指向具体的完成实现工作的类 PersonImpl  
      
};  

Person::Person(const std::string& name,const int &id,const std::string& addr)  
    :pImpl(new PersonImpl(name,id,addr))  
{  
}  
std::string Person::rename() const  
{  
    return pImpl->rename(); //通过指针调用实现类里的成员函数，完成具体动作  
}  
int Person::reid() const  
{  
    return pImpl->reid();  
}  
std::string Person::readdr() const  
{  
    return pImpl->readdr();  
}  


  

PersonImpl::PersonImpl(const std::string& name,const int &id,const std::string& addr)  
    :theName(name),theId(id),theAddress(addr)  
{  
}  
std::string PersonImpl::rename() const  
{  
    return theName;  
}  
int PersonImpl::reid() const  
{  
    return theId;  
}  
std::string PersonImpl::readdr() const  
{  
    return theAddress;  
}  
//Main.cpp  
// #include <iostream>  
// #include "Person.h"  
//主程序，只需要包含Person类，因为Person类的接口永远不会改变，如果实现文件PersonImpl中的接口或者成员改变了  
//只需要重新编译Person.h，而不需要重新编译包含person.h的文件了  
int main()  
{  
    Person man1("Tom",10,"England");  
    std::cout<<"the name:"<<man1.rename()<<std::endl;  
    std::cout<<"the id:"<<man1.reid()<<std::endl;  
    std::cout<<"the Address:"<<man1.readdr()<<std::endl;  

    return 0;  
}
