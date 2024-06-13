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
int main()
{
    MyBase31 a(1, 2, 3);
    MyBase32 b(4, 5, 6);
}