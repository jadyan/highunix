#include <iostream>
#include <memory>
#include <sstream>
#include "Lock.h"

/**
 *条款29：为”异常安全“而努力是值得的

**不泄露任何资源：当 new Image(imgSrc) 导致异常，unlock 则绝的不会被执行，导致了锁资源的泄漏；
**不允许数据败坏：当 new Image(imgSrc) 导致异常，bgImage 就是指向一个已被删除的对象，并且 imageChanges 也被累加；
 *
 *
 * 异常安全函数（Exception-safe functions）提供以下三个保证之一：
 *
 * 基本承诺：如果异常被抛出，程序内的任何事物仍然保持在有效状态；
 * 强烈保证：如果异常被抛出，程序状态不变，即函数成功就是完全成功，函数失败程序就会回复到”调用函数之前的状态“；
 * 不抛掷（nothrow）保证：承诺绝对不抛出异常，因为它们总是能够完成它们原先承诺的功能
 *
 * ***/
class Image
{
public:
	Image(std::istream& i) : img(i) {}

private:
	std::istream& img;
};

// Pretty Menu impl.
struct PMImpl
{
	std::shared_ptr<Image> bgImage;
	int imageChanges;
};

class PrettyMenu
{
public:
	void changeBackground(std::istream& imgSrc);

private:
	Mutex mutex;
	std::shared_ptr<PMImpl> pImpl;
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	using std::swap;
	Lock m1(&mutex);
	/*
	在修改一个对象时，可以先创建一个副本，然后在副本身上做一切必要的修改；
	若有任何修改动作抛出异常，原对象仍保持未改变的状态；待所有改变成功后，再将修改过的那个副本和原对象在一个不抛出异常的操作中置换。
	*/
	//pImpl = std::shared_ptr<PMImpl>  (new PMImpl()); 
	if (pImpl == nullptr){
		std::cout<<"初始化"<<std::endl;
		pImpl = std::shared_ptr<PMImpl>  (new PMImpl()); 
	}else{
		std::cout<<"替换"<<std::endl;
		std::shared_ptr<PMImpl> pNew(new PMImpl(*pImpl));
		pNew->bgImage.reset(new Image(imgSrc));
		++pNew->imageChanges;
		swap(pImpl, pNew);
	}
	 

}

int main()
{
	std::istream* img = NULL;
	PrettyMenu pm;
	pm.changeBackground(*img);
	return 0;
}
