#include <iostream>
#include <stdexcept>
using namespace std;

class TestException : public runtime_error
{
public:
    TestException() : runtime_error("abnormal program termination") {}
};

int main()
{
    try
    {
        throw TestException();
    }
    catch (TestException &testexception)
    {
        cout << "This is a test\n"
             << endl;
        cout << testexception.what() << endl;
    }
}