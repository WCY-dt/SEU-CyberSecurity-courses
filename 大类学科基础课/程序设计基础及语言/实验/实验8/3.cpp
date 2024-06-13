#include <iostream>
using namespace std;
template <typename T>
class Vector
{
private:
    int size;
    T *data;

public:
    Vector(int input_size) : size(input_size)
    {
        data = new T[input_size];
        for (int i = 0; i < input_size; i++)
        {
            data[i] = 0;
        }
    }
    friend ostream &operator<<(ostream &out, Vector<T> k)
    {
        for (int i = 0; i < k.size; i++)
        {
            out << k.data[i] << " ";
        }
        return out;
    }
    friend istream &operator>>(istream &in, Vector<T> k)
    {
        for (int i = 0; i < k.size; i++)
        {
            in >> k.data[i];
        }
        return in;
    }
};
int main()
{
    Vector<int> a(5);
    cout << "Enter 5 integers:";
    cin >> a;
    cout << a;
}