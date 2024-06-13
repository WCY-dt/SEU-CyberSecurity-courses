#include<iostream>
#include<set>
using namespace std;

bool ifPrime(int a){
    bool isprime = 1;
    for(int i=2;i<a;i++){
        if (a%i==0){
            isprime = 0;
            break;
        }
    }
    return isprime;
}

set<int> s;
void factorize(int a)
{
	while (!ifPrime(a))
	{
		for (auto p = s.begin(); p != s.end(); p++)
		{
			if (a % *p == 0)
			{
				a = a / *p;
				cout << *p << '*';
				break;
			}
		}
	}
	cout << a << endl;
}

int main(){
    cout << "The prime numbers in the range 2 to 1023 are:" << endl;
    for (int k=2;k<=1024;k++){
        if (ifPrime(k)){
            cout << k << " ";
            s.insert(k);
        }
    }
    cout << endl;

    while (true)
    {
        int j;
        cout << "Enter a value from 1 to 1023: ";
        cin >> j;
        if (ifPrime(j))
        {
            cout << j << " is a prime number" << endl;
        }
        else
        {
            cout << j << " is not a prime number" << endl;
            cout << "The unique prime factorization of " << j << " is: ";
            factorize(j);
        }
    }

}