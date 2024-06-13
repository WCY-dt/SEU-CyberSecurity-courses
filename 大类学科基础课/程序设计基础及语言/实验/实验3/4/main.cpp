#include <iostream>
using std::cout;
using std::endl;

class MyBase
{
    int x;

public:
    MyBase(int a) : x(a){}
    int getX()
    {
        return x;
    }
};

class MyDerived : public MyBase
{
    int y;

public:
    MyDerived(int a) : y(a), MyBase(a + 4){}
    int getY()
    {
        return y;
    }
};

int main()
{
    MyBase a(2), *p = &a;
    MyDerived b(4), *q = &b;
    MyBase &c = a;
    MyBase &d = b;
    cout << a.getX() << " " << p->getX() << endl;
    cout << b.getY() << " " << q->getY() << b.getX() << " " << q->getX() << endl;
    a = b;
    cout << a.getX() << " " << endl; //<< a.getY()
    p = q;
    cout << p->getX() << " " << endl;                   //<< p->getY()
    cout << c.getX() << " " << d.getX() << " " << endl; //<< d.getY()
    //b = a;
    cout << b.getX() << " " << b.getY() << endl;
}