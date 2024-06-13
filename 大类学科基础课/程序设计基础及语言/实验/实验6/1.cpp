#include <iostream>
#include <vector>
using namespace std;

template <class T>
void check(const vector<T> a)
{
    int b = a.size() / 2 + 1;
    bool judge = true;
    for (int i = 0; i < b; i++)
    {
        if (a[i] != a[a.size() - i - 1])
        {
            judge = false;
        }
    }
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i] << " ";
    }
    cout << "is" << (judge ? " " : " not ") << "a palindrome" << endl;
}

int main()
{
    vector<int> a = {75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65};
    vector<char> b{'K', 'J', 'I', 'H', 'G', 'F', 'G', 'H', 'I', 'J', 'K'};
    check(a);
    check(b);
}