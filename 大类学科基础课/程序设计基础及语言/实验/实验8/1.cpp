#include <iostream>
#include <string>
using namespace std;
template <typename elementType, int numberOfElement>
class Array
{
public:
    Array(elementType a[numberOfElement])
    {
        for (int i = 0; i < numberOfElement; i++)
            Elements[i] = a[i];
    };
    void print()
    {
        for (int i = 0; i < numberOfElement; i++)
            cout << Elements[i] << " ";
        cout << endl;
    }

private:
    elementType Elements[numberOfElement];
};
int main()
{
    int A[5];
    cout << "Enter 5 integer values:" << endl;
    for (int i = 0; i < 5; i++)
        cin >> A[i];
    cout << "The values in the intArray are:" << endl;
    Array<int, 5> a(A);
    a.print();
    string B[7];
    cout << "Enter 7 one word string " << endl;
    for (int i = 0; i < 7; i++)
        cin >> B[i];
    cout << "The values in the stringArray are:" << endl;
    Array<string, 7> b(B);
    b.print();
}