/*
 * @Author: your name
 * @Date: 2021-03-15 20:28:12
 * @LastEditTime: 2021-03-16 21:47:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/04_defult_constructor.cpp
 */
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class EquipmentPiece {
public:
EquipmentPiece(int IDNumber):data(IDNumber){};
private:
  int data;
};

int main(int argc, char **argv)
{
    //EquipmentPiece  bestPieces[10];
    //
    //EquipmentPiece *p = new EquipmentPiece[10];
     int ID1, ID2, ID3, ID4,ID5,ID6,ID7,ID8,ID9, ID10; // 存储设备ID号的变量

    EquipmentPiece bestPieces[] = { EquipmentPiece(ID1), 
    EquipmentPiece(ID2),
    EquipmentPiece(ID3),
    EquipmentPiece(ID4),
    EquipmentPiece(ID5),
    EquipmentPiece(ID6),
    EquipmentPiece(ID7),
    EquipmentPiece(ID8),
    EquipmentPiece(ID9),
    EquipmentPiece(ID10)
    };

    typedef EquipmentPiece* PEP; // PEP指针指向一个EquipmentPiece对象
	PEP bestPieces4[10]; // 正确，没有调用构造函数

    PEP* bestPieces5 = new PEP[10]; // 也正确
	// 在指针数组里的每一个指针被重新赋值，以指向一个不同的EquipmentPiece对象
	for (int i = 0; i < 10; ++i)
		bestPieces5[i] = new EquipmentPiece(ID1);


    

    // 为数组分配raw memory,可以避免浪费内存，使用placement new方法在内存中构造EquipmentPiece对象
	void* rawMemory = operator new[](10*sizeof(EquipmentPiece));
	// make bestPieces6 point to it so it can be treated as an EquipmentPiece array
	EquipmentPiece* bestPieces6 = static_cast<EquipmentPiece*>(rawMemory);
	// construct the EquipmentPiece objects in the memory使用"placement new"
	for (int i = 0; i < 10; ++i)
		new(&bestPieces6[i]) EquipmentPiece(ID1);

    	// 以与构造bestPieces6对象相反的顺序解构它
	for (int i = 9; i >= 0; --i)
		bestPieces6[i].~EquipmentPiece(); // 如果使用普通的数组删除方法，程序的运行将是不可预测的
	// deallocate the raw memory
	delete [] rawMemory;
  return 0;
}