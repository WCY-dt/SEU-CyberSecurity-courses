# EX1

```cpp
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
```

<img src="E:/20-21-2/%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1/%E5%AE%9E%E9%AA%8C/%E5%AE%9E%E9%AA%8C7/image-20210525133308945.png" alt="image-20210525133308945" style="zoom:50%;" />

# EX2

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;
class Item
{
public:
    Item(int x = 3) : value(x)
    {
        cout << "Item " << value << " constructor called.\n";
        if (value == 3)
        {
            throw runtime_error("An exception was thrown");
        }
    }

    ~Item()
    {
        cout << "Item " << value << " destructor called.\n";
    }

private:
    int value;
};

int main()
{
    try
    {
        Item item1(1);
        Item item2(2);
        Item item3(3);
    }
    catch (runtime_error &exception)
    {
        cerr << exception.what() << endl;
    }
}
```

<img src="E:/20-21-2/%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1/%E5%AE%9E%E9%AA%8C/%E5%AE%9E%E9%AA%8C7/image-20210525133432034.png" alt="image-20210525133432034" style="zoom:50%;" />

# EX3

```cpp
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
```

<img src="E:/20-21-2/%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1/%E5%AE%9E%E9%AA%8C/%E5%AE%9E%E9%AA%8C7/image-20210525133549787.png" alt="image-20210525133549787" style="zoom:50%;" />

# EX4

```cpp
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
```

<img src="E:/20-21-2/%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1/%E5%AE%9E%E9%AA%8C/%E5%AE%9E%E9%AA%8C7/image-20210525133702850.png" alt="image-20210525133702850" style="zoom:50%;" />

