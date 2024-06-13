#include <iostream>
class MyBase1
{
public:
    MyBase1()
    {
        std::cout << "…BaseClass1 Object is created!" << std::endl;
    }
    ~MyBase1()
    {
        std::cout << "…BaseClass1 Object is destroyed!" << std::endl;
    }
};
class MyBase2
{
    MyBase1 a1;

public:
    MyBase2()
    {
        std::cout << "…BaseClass2 Object is created!" << std::endl;
    }
    ~MyBase2()
    {
        std::cout << "…BaseClass2 Object is destroyed!" << std::endl;
    }
};
class MyDerived1 : public MyBase2
{
    MyBase1 a1;

public:
    MyDerived1()
    {
        std::cout << "…First layer derived Object is created!" << std::endl;
    }
    ~MyDerived1()
    {
        std::cout << "…First layer derived Object is Destroyed!" << std::endl;
    }
};
class MyDerived11 : public MyDerived1
{
public:
    MyDerived11()
    {
        std::cout << "…Second layer derived Object is created!" << std::endl;
    }
    ~MyDerived11()
    {
        std::cout << "…Second layer derived Object is destroyed!" << std::endl;
    }
};
int main()
{
    MyBase2 a;
    MyDerived1 b;
    MyDerived11 c;
}