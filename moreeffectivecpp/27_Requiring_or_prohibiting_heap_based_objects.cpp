/*
 * @Author: your name
 * @Date: 2021-05-07 10:07:20
 * @LastEditTime: 2021-05-07 10:29:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/27_Requiring_or_prohibiting_heap_based_objects.cpp
 */
#include <iostream> 
using namespace std;


/*
如果要一个对象必须在堆中建立，即禁止new 操作以外的手段建立对象，防止被自动构造和自动释放，
最简单的办法是禁止使用隐式的构造函数和析构函数，比如把构造函数和析构函数声明为private。
如果两者都声明为私有，会增加工作量，要定义自己的伪构造函数和伪析构函数，更好的办法是让析构函数成为private，
让构造函数成为public。引进一个专用的伪析构函数，用来访问真正的析构函数。客户端调用伪析构函数释放他们建立的对象。
*/


class UPNumber 
{
public:
    UPNumber() 
    { 
        cout << "UPNumber 构造" << endl; 
    }
    virtual void destroy() const 
    { 
        delete this; 
    } 

protected:
    ~UPNumber() 
    { 
       cout << "UPNumber 析构" << endl; 
    } 


}; 

class NegativeUPNumber : public UPNumber
{
#if 1
  public:
    NegativeUPNumber():UPNumber()
    {
        cout<<"NegativeUPNumber 构造"<<endl;
    }

    virtual void destroy() const 
    { 
        delete this; 
    }
  private:
    ~NegativeUPNumber() 
    { 
       cout << "NegativeUPNumber 析构" << endl; 
    } 
#endif
}; 


int main()
{
  //UPNumber n; 
  UPNumber *p = new UPNumber(); 
  //delete p; 
  p->destroy(); 

#if 1
  cout<<"子类："<<endl;
  //NegativeUPNumber n; 
  NegativeUPNumber *n2 = new NegativeUPNumber; 
  //delete n2; 
  n2->destroy();
#endif

}