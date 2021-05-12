#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

class NLComponent  //抽象基类
{
public:
    virtual ostream& print(ostream& s) const = 0;
    virtual ~NLComponent(){}
};

class TextBlock : public NLComponent  //子类一
{
public:
    TextBlock(string name): name(name){}
    ~TextBlock();
    virtual ostream& print(ostream& s) const ;
private:
    string name;  //名称
};

ostream& TextBlock::print(ostream& s) const
{
    s<<"TextBlock : "<<name<<endl;
    return s;
}

TextBlock::~TextBlock()
{
    cout<<"TextBlock Destroy"<<endl;
}

class Graphic : public NLComponent  //子类二
{
public:
    Graphic(string name): name(name){}
    ~Graphic();
    virtual ostream& print(ostream& s) const ;
private:
    string name;   //名称
};

ostream& Graphic::print(ostream& s) const
{
    s<<"Graphic : "<<name<<endl;
    return s;
}

Graphic::~Graphic()
{
    cout<<"Graphic Destroy"<<endl;
}

inline ostream& operator<<(ostream& s, const NLComponent& c)
{
    cout<<"operator<< 函数"<<endl;
    return c.print(s);
}

class NewLetter
{
public:
    NewLetter(ifstream &fin);
    ~NewLetter();
    void printAll();
private:
    list<NLComponent*> components;
};

NewLetter::NewLetter(ifstream &fin)
{
    string str;
    while (fin >> str)   //根据不同的信息，构造不同的子类对象，即构造函数虚拟化
    {
        cout<<"当前字符串: "<<str<<endl;
        if (-1 != str.find("TextBlock"))
        {
            components.push_back(new TextBlock(str));
        }
        else if (-1 != str.find("Graphic"))
        {
            components.push_back(new Graphic(str));
        }
    }
}

NewLetter::~NewLetter()
{
    for (list<NLComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
    {
        delete (*it);
    }
    components.clear();
}

void NewLetter::printAll()
{
    for (list<NLComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
    {
        cout<<**it;    //直接输出不同子类的信息，即虚拟化非成员函数
        //(*it)->print(cout);
    }
}

int main(int argc, char** argv)
{
    ifstream fin("components.txt");
    NewLetter *letter = new NewLetter(fin);
    letter->printAll();
    delete letter;
    return 0;
}