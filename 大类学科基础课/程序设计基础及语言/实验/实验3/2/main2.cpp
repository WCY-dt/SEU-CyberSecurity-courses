#include <iostream>
class MyBase31
{
    int a, b, c;

public:
    MyBase31(int x, int y, int z) : a(x), b(y), c(z)
    {
        std::cout << "…BaseClass31 Object is created!" << std::endl;
        std::cout << a << " " << b << " " << c << std::endl;
    }
    ~MyBase31()
    {
        std::cout << "…BaseClass31 Object is destroyed!" << std::endl;
    }
};
class MyBase32
{
    int a, b, c;

public:
    MyBase32(int x, int y, int z)
    {
        std::cout << "…BaseClass32 Object is created!" << std::endl;
        std::cout << a << " " << b << " " << c << std::endl;
        a = x, b = y, c = z;
        std::cout << a << " " << b << " " << c << std::endl;
    }
    ~MyBase32()
    {
        std::cout << "…BaseClass32 Object is destroyed!" << std::endl;
    }
};
class MyDerived1 : public MyBase31
{
    MyBase31 a = MyBase31(5, 6, 7);
    int c;

public:
    MyDerived1(int x) : c(x), MyBase31(x, 8, 9)
    {
        std::cout << "…Base Object has been created!" << std::endl;
        std::cout << "…Member Object has been created! " << std::endl;
        std::cout << "…Derived Object is created! " << c << std::endl;
    }
};
int main()
{
    MyDerived1 b(88);
}