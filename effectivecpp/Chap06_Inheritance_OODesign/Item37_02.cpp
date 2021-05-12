#include <iostream>
using namespace std;


class Shape
{
public:
	enum ShapeColor { Red, Green, Blue };

	// All shapes must offer a function to draw themselves.
	virtual void draw(ShapeColor color = Blue) const = 0;
};


class Rectangle : public Shape
{
public:
	// Notice the different default parameter value = bad!
	virtual void draw(ShapeColor color = Green) const
	{
		//如果重新定义了继承而来的缺省参数值，可能会在”调用一个定义于 derived class 内的 virtual 函数“时，使用 base class 为它所指定的缺省参数值
		cout<<"Rectangle::draw : "<<color<<endl;
	}
};


class Circle : public Shape
{
public:
	virtual void draw(ShapeColor color) const
	{
		cout<<"Circle::draw : "<<color<<endl;
	}
};

int main()
{
	Shape* pr = new Rectangle;	// static type = Shape*		dynaminc type = Rectangle*
	Shape* pc = new Circle;		// static type = Shape*		dynaminc type = Circle*


	
	// Calls Shape::draw(Shape::Blue);
	// Calls Rectangle::doDraw(Shape::Blue);
	//pr->draw(Shape::Blue);
	pr->draw();

	// Calls Shape::draw();
	// Calls Rectangle::doDraw(Shape::Red);
	pc->draw();


	// Calls Shape::draw(Shape::Red);
	// Calls Circle::draw(Shape::Red);
	//pc->draw(Shape::Red);

	// Calls Shape::draw();
	// Calls Circle::draw(Shape::Red);
	//pc->draw();


	delete pc;
	delete pr;

	return 0;
}