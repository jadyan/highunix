#include <iostream>
#include <memory>
using namespace std;


/*
 *
 * 条款28：避免返回 handles 指向对象内部成分
 *
 * */

class GuiObject
{
};

// class for representing points
class Point
{
public:
	Point() {}
	Point(int xx, int yy) : x(xx), y(yy) {}
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void print() const
	{
		cout<<"Point print 函数"<<endl;
		//cout<<"x="<<this->x<<endl;
		//cout<<"y="<<this->y<<endl;
	}

private:
	int x, y;
};

// Point data for a Rectangle
struct RectData
{
	//RectData() {}
	RectData(Point p1, Point p2)
	{
		ulhc = p1;
		lrhc = p2;
	}
	Point ulhc;	// upper left hand corner
	Point lrhc;	// lower rght hand corner
};

class Rectangle
{
public:
	Rectangle() 
	{
		cout<<"Rectangle构造"<<endl;
	}
	Rectangle(Point p1, Point p2) : pData(create(p1, p2))
	{
	}

	const Point& upperLeft() const 
	{
		cout<<"Point upperLeft 函数"<<endl;
		return pData->ulhc; 
	}
	const Point& lowerRight() const { return pData->lrhc; }

private:
	std::shared_ptr<RectData> pData;
	RectData* create(Point p1, Point p2)
	{
		return new RectData(p1, p2);
	}
};


// return a rectangle by value (Item3 return const)
const Rectangle boundingBox(const GuiObject& obj)
{
	Rectangle r;
	return r;
}

int main()
{
	// make pgo point to some GUI object
	GuiObject* pgo = new GuiObject;

	// get ptr to the upper left point of its bounding box
	const Point* pUpperLeft = &(boundingBox(*pgo)).upperLeft();
	pUpperLeft->print();

	delete pgo;
	return 0;
}


