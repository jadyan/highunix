#include <iostream>
#include <sstream>

class Mutex
{
};

void lock(const Mutex& mutex)
{
}
void unlock(const Mutex& mutex)
{
}

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
	void changeBackground(std::istream& imgSrc);
	PrettyMenu():imageChanges(0),bgImage(nullptr){}
private:
	Mutex mutex;
	Image* bgImage;
	int imageChanges;
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	lock(mutex);
	if (bgImage == nullptr)
	{
		bgImage = new Image(imgSrc);
	}else{
		delete bgImage;
		++imageChanges;
		bgImage = new Image(imgSrc);
	}

	unlock(mutex);
}

int main()
{
	std::istream* img = NULL;
	PrettyMenu pm;
	pm.changeBackground(*img);
	return 0;
}
