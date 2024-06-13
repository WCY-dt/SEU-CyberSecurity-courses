#include <cstdio>
#include <ctime>
#include <iostream>
using namespace std;

long long prime[10]={2,3,5,7,11,13,17,19,23,29};
long long p,q;
//const long long p = 10191161;
//const long long q = 10191133;
long long n;

void ex_gcd(long long a, long long b, long long &x, long long &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return;
    }
    ex_gcd(b, a % b, x, y);
    long long t = x;
    x = y;
    y = t - a / b * y;
    return;
}

long long qMultiply(long long x, long long y, long long c)
{
    return (x * y - (long long)((long double)x / c * y) * c + c) % c;
}

long long qPower(long long base, long long power, long long c)
{
    long long ans = 1, res = base;
    while (power)
    {
        if (power & 1) //power is even
            ans = qMultiply(ans, res, c) % c;
        res = qMultiply(res, res, c) % c;
        power >>= 1; //power/2
    }
    return ans;
}

bool is_coprime(long long a, long long b)
{
    if (a == 1 || b == 1)
        return true;
    while (true)
    {
        long long t = a % b;
        if (t == 0)
            break;
        else
        {
            a = b;
            b = t;
        }
    }
    return (b == 1);
}

bool mr(long long number)//Miller-Rabin
{
    long long a,b,s=0,t=number-1;
    
    while(!(t&1))
    {
        s++;
        t>>=1;
    }
    
    for(long long i=0;i<10;i++)
    {
        a=qPower(prime[i],t,number);
        for(long long j=1;j<=s;j++)
        {
            b=(a*a)%number;
            if(b==1 && a!=1 && a!=number-1)
              return false;
            a=b;
        }
        if(a!=1)
			return false; 
    }
    return true;
}

long long RSA_encode(long long m, long long &d, long long &e, long long &phi_n)
{
    long long temp;
    n = p * q;
    phi_n = n - p - q + 1;
    e = rand() % phi_n + 1;
    while (!is_coprime(e, phi_n))
        e = rand() % (phi_n - 2) + 2;
    ex_gcd(e, phi_n, d, temp);
    while (d <= 0)
        d += phi_n;
    return (qPower(m, e, n));
}

long long RSA_decode(long long n, long long d, long long c)
{
    return (qPower(c, d, n));
}

int main()
{
    srand(time(0));
    printf("\n------------------------------------------------------------\n");
    printf("ENCODE BEGIN:");
    printf("\n------------------------------------------------------------\n");
    long long m, d, e, phi_n, c, Message;
    printf("\nInput your plain text: ");
    scanf("%lld", &m);
    p=rand();
    long long prand=rand()%20+1;
    while (p<(prand*m)) p+=rand();
    q=rand();
    long long qrand=rand()%20+1;
    while (q<(qrand*m)) q+=rand();
    while (!mr(p)) p++;
    while (!mr(q)) q++;
    printf("\nPrimes are p=%lld, q=%lld\n",p,q);
    c = RSA_encode(m, d, e, phi_n);
    printf("\nPK is (%lld,%lld)\nSK is (%lld,%lld)\n", e, n, d, n);
    printf("\nCipher text is %lld\n", c);

    printf("\n------------------------------------------------------------\n");
    printf("DECODE BEGIN:");
    printf("\n------------------------------------------------------------\n");
    Message = RSA_decode(n, d, c);
    printf("\nPlain text is %lld\n\n", Message);
}