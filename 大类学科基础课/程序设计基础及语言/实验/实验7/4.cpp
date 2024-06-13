#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
class TestException : public runtime_error
{
public:
    TestException(string s = "") : runtime_error(s) {}
};
void f()
{
    throw TestException("TestException");
}
void g()
{
    f();
}
void h()
{
    g();
}
int main()
{
    try
    {
        h();
    }
    catch (TestException &ex)
    {
        cout << "In main: Caught ";
        cerr << ex.what();
    }
}