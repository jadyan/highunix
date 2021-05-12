#include <iostream>


/*
Item 32: 确保 public inheritance 模拟 "is-a"
public inheritance 意味着 "is-a"。适用于 base classes 的每一件事也适用于 derived classes，因为每一个 derived class object 都是一个 base class object。

*/

class Person
{
};

class Student : public Person
{
};


class Rectangle
{
public:
	virtual int getHeight() const { return height; }
	virtual int getWidth() const { return width; }

	virtual void setHeight(int newHeight) { height = newHeight; }
	virtual void setWidth(int newWidth) { width = newWidth; }

private:
	int height;
	int width;
};


class Square : public Rectangle
{
};


// Increase r area.
void makeBigger(Rectangle& r)
{
	int oldHeight = r.getHeight();
	int oldWidth = r.getWidth();

	r.setWidth(oldWidth + 10);
	int newHeight = r.getHeight();

	//assert(newHeight, oldHeight);
}

int main()
{
    Rectangle r;
    makeBigger(r);
	return 0;
}

