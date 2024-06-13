#include "IntegerSet.h"

#include<iostream>
using std::cout;
using std::endl;
const int arraySize=10;

int main()
{
	IntegerSet A;
    IntegerSet B;
    IntegerSet C;

    cout<<"Enter set A:\n";
    A.inputSet();
    cout<<"Enter set B:\n";
    B.inputSet();

    cout<<"Union of A and B is:\n";
    C=A.unionOfSets(B);
    C.printSet();

    cout<<"intersection of A and B is:\n";
    C=A.intersectionOfSets(B);
    C.printSet();

    A.printEqual(A.isEqualTo(B));
    cout<<endl;

    cout<<"insert 77 into set A...\n";
    A.insertElement(77);
    cout<<"Set A is now :\n";
    A.printSet();
    cout<<endl;

    cout<<"delete 77 from A...\n";
    A.deleteElement(77);
    cout<<"Set A is now :\n";
    A.printSet();
    A.deleteElement(101);
    A.deleteElement(50);

    const int arraySize=10;
    int intArray[arraySize]={25,67,2,9,99,105,45,-5,101,1};
    IntegerSet e(intArray,arraySize);

    cout<<"\nSet e is:\n";
    e.printSet();
}
