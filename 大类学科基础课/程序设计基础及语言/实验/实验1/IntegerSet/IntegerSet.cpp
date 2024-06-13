#include "IntegerSet.h"

#include<iostream>
using std::cin;
using std::cout;
using std::endl;

IntegerSet::IntegerSet(int* num,int n)
{
    for (int i=0;i<n;++i)
        k[num[i]]=1;
}

void IntegerSet::inputSet()
{
    int inp=0;
    cout<<"Enter an element ('-1' to end) :";
    cin>>inp;
    while (inp!=-1)
	{
        k[inp]=1;
        cout<<"Enter an element ('-1' to end) :";
        cin>>inp;
    }
    cout<<"Entry complete\n";
    cout<<endl;
}

void IntegerSet::printSet() const
{
    cout<<"{ ";
    for (int i=0;i<101;++i)
        if(k[i]==1)
            cout<<i<<" ";
    cout<<"}"<<endl;
}

bool IntegerSet::isEqualTo(IntegerSet a)const
{
        for (int i=0;i<101;++i)
            if(k[i]!=a.k[i])
                return false;
    return true;
}

void IntegerSet::insertElement(int n)
{
    k[n]=1;
}

IntegerSet IntegerSet::unionOfSets(IntegerSet a) const
{
    IntegerSet added=a;
    for (int i=0;i<101;++i)
        if(k[i]==1)
            added.k[i]=1;
    return added;
}

IntegerSet IntegerSet::intersectionOfSets(IntegerSet a) const
{
    IntegerSet iso;
    for (int i=0;i<101;++i)
        if(a.k[i]==1 && this->k[i]==1)
            iso.k[i]=1;
    return iso;
}

void IntegerSet::deleteElement(int n)
{
    if(k[n]==1)
        k[n]=0;
    else
        cout<<"Invalid insert attempted!"<<endl;
}

void IntegerSet::printEqual(bool a)
{
    if(a==1)
        cout<<"A is equal to B\n";
    else
        cout<<"A is not equal to B\n";
}
