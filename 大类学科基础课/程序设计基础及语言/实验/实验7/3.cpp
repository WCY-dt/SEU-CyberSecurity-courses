#include <iostream>
#include <stdexcept>
using namespace std;
class TestException : public runtime_error
{
public:
    TestException() : runtime_error("abnormal program termination") {}
};
void g()
{
    try
    {
        throw TestException();
    }
    catch (TestException &)
    {
        cerr << "Exception caught in function g(). Rethrowing...\n";
        throw;
    }
}
int main()
{
    try
    {
        g();
    }
    catch (TestException &)
    {
        cout << "Exception caught in function main()." << endl;
    }
}