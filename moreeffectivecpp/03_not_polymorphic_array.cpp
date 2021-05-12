/*
 * @Author: your name
 * @Date: 2021-03-10 21:31:09
 * @LastEditTime: 2021-03-10 21:47:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/03_not_polymorphic_array.cpp
 */
//不要对数组使用多态
#include <iostream>
using namespace std;

class BST
{
public:
	int a;
	BST(){a = N;N++;}
	friend ostream& operator<<(ostream &os,const BST& bst)
	{
		os << bst.a << ",";
		return os;
	}
	~BST(){cout << "dtor BST ... "<<  a  << endl;}
	static int N;
};

int BST::N = 0;

class BalancedBST:public BST
{
public:
	BalancedBST(){b = 2;}
	~BalancedBST(){cout << "dtor BalancedBST ... " << endl;}
private:
	int b;
};


/*
编译器要想遍历数组中每一个元素，它必须知道每一个元素的大小。很明显，当print参数为BalancedBST数组时，
编译器静态的将其数组大小当作BST的大小处理，以*(i+arr)的方式前进，结果是未知的。
*/
void PrintBSTArray(ostream& s,const BST array[],int numElements)
{
	for(int i=0;i<numElements;++i)
		s << array[i];
}


#if 0 
void PrintBSTArray2(ostream& s,const BalancedBST array[],int numElements)
{
	for(int i=0;i<numElements;++i)
		s << array[i];
}
#endif

void DeleteBST(ostream&os,BST array[])
{
	os << "Deleting array at address " << static_cast<void*>(array) << endl;	
	delete[] array; //这里每个array[i]对应的地址对么？
}

void DeleteBSTP(ostream&os,BST *array)
{
	os << "Deleting array at address " << static_cast<void*>(array) << endl;	
	delete[] array; //这里每个array[i]对应的地址对么？
}

int main()
{
	BST Array[10];
	PrintBSTArray(cout,Array,10);
	cout << endl;
	//如果数组是子类
	BalancedBST bArray[10];
	PrintBSTArray(cout,bArray,10); //这个时候输出就不对了
	cout << endl;
   
	//如果删除数组呢？
	//DeleteBST(cout,Array);
	//DeleteBST(cout,bArray); //死循环掉了


	BST *parray = new BalancedBST[10];
	DeleteBSTP(cout,parray);
	//DeleteBSTP(cout,bArray);
	//DeleteBST(cout,parray);

	
}