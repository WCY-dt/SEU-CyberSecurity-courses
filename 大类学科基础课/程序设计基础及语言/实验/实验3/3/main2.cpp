#include <iostream>
using std::cout;
using std::endl;

class MyBase3
{

protected:
    int y;
    void fun2()
    {
        cout << "MyBase3---fun2()" << endl;
    }

public:
    int x;
    void fun1()
    {
        cout << "MyBase3---fun1()" << endl;
    }
    int z;
    void MyBase(int a, int b, int c)
    {
        x = a;
        y = b;
        z = c;
    }

    int getX()
    {
        cout << "MyBase3---x:";
        return x;
    }

    int getY()
    {
        cout << "MyBase3---y:";
        return y;
    }

    int getZ()
    {
        cout << "MyBase3---z:";
        return z;
    }

    void fun3()
    {
        cout << "MyBase3---fun3()" << endl;
    }
};

class MyDerived2 : public MyBase3
{
public:
    int p;
    MyDerived2(int a) : p(a) {}
    int getP()
    {
        cout << "MyDerived---p:";
        return p;
    }

    void display()
    {
        cout << p << " " << x << " " << y << " " << z << " " << endl;
        fun1();
        fun2();
        fun3();
    }
};

class MyDerived21 : public MyBase3
{
public:
    int p;
    MyDerived21(int a) : p(a) {}
    int getP()
    {
        cout << "MyDerived21---p:" << endl;
        return p;
    }
    void display1()
    {
        cout << p << " " << x << " " << y << " " << z << " " << endl;
    }
};

int main()
{
    MyDerived2 a(3);
    MyDerived21 b(6);
    a.display();
    //cout << a.x << " " << a.p << " " << a.y << " " << a.z << endl;
    cout << a.getX() << "\n"
         << a.getP() << "\n"
         << a.getY() << "\n"
         << a.getZ() << endl;
    b.display1();
}