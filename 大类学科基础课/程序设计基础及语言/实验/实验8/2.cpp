#include <iostream>
using namespace std;
template <typename T>
bool isEqualTo(T a, T b)
{
    return a == b;
}
class Complex
{
public:
    Complex(double input_real = 0, double input_imagine = 0) : real(input_real), imagine(input_imagine) {}
    double getR()
    {
        return real;
    }
    double getI()
    {
        return imagine;
    }
    bool operator==(Complex a)
    {
        return real == a.real && imagine == a.imagine;
    }

private:
    double real, imagine;
};
int main()
{
    int a = 1, b = 2;
    cout << "a is " << (isEqualTo(a, b) ? "" : "not") << " equal to b" << endl;
    double x = 3.14, y = 6.28;
    cout << "x is " << (isEqualTo(x, y) ? "" : "not") << " equal to y" << endl;
    char c = 'A', d = 65;
    cout << "c is " << (isEqualTo(c, d) ? "" : "not") << " equal to d" << endl;
    Complex e(2, 3), f(3, 3);
    cout << "Complex a is " << (isEqualTo(e, f) ? "" : "not") << " equal to complex b" << endl;
}