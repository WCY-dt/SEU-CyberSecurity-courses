# Ex1

**1.cpp**

```cpp
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
```

**output**

<img src="E:/tmp/TyporaPic/image-20210511204713411.png" alt="image-20210511204713411" style="zoom:50%;" />

# Ex2

**2.cpp**

```cpp
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
```

**output**

<img src="E:/tmp/TyporaPic/image-20210511205011037.png" alt="image-20210511205011037" style="zoom:50%;" />

