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