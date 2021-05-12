// Authors: Handling
// Created by 15870 on 2020/12/11.
// This is 了解new 和 delete 的合理替换时机

/* 总结：
 * 1. 为了检测运用错误
 * 2. 为了收集动态分配内存之使用统计信息
 * 3. 为了强化效能
 * 4. 为了增加分配和归还的速度
 * 5. 为了降低缺省内存管理器带来的空间额外开销
 * 6. 为了弥补缺省分配器中的非最佳齐位
 * 7. 为了将相关对象成簇集中 （减少内存页错误的出现频率）
 * 8. 为了获得非传统的行为
 */

#include <stdio.h>
#include <iostream>
//#include <new>

static const int signature = 0xDEADBEEF;
typedef unsigned char Byte;

// this code has several flaws!
void* operator new(std::size_t size) throw(std::bad_alloc)
{
	using namespace std;
    cout<<"operator new 函数"<<endl;
	// increase size of request so 2x signatures will also fit inside
	size_t realSize = size + 2 * sizeof(int);

	// call malloc to get the actual memory
	void* pMem = malloc(realSize);
	if (!pMem)
	{
		throw bad_alloc();
	}

	// write signature into first and last parts of the memory
	*(static_cast<int*>(pMem)) = signature;
	*(reinterpret_cast<int*>(static_cast<Byte*>(pMem) + realSize - sizeof(int))) = signature;

	// return a pointer to the memory just past the first signature
	return static_cast<Byte*>(pMem) + sizeof(int);
}

int main()
{
    std::size_t size=1024;
    void * p = operator new (size);
	return 0;
}
