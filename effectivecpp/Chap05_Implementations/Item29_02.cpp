#include <iostream>
#include <sstream>
#include "Lock.h"

class Image
{
public:
	Image(std::istream& i) : img(i) {}

private:
	std::istream& img;
};

class PrettyMenu
{
public:
	PrettyMenu():imageChanges(0),bgImage(nullptr){}
	void changeBackground(std::istream& imgSrc);

private:
	Mutex mutex;
	Image* bgImage;
	int imageChanges;
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	Lock m1(&mutex);
	if (bgImage == nullptr){
		bgImage = new Image(imgSrc);
		++imageChanges;
	}else{
		delete bgImage;
		++imageChanges;
		bgImage = new Image(imgSrc);
	}

}

int main()
{
	std::istream* img = NULL;
	PrettyMenu pm;
	pm.changeBackground(*img);
	return 0;
}
