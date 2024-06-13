#include <cstdio>
using namespace std;

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

long long gcd(long long a, long long b)
{
    if (a % b == 0)
        return b;
    return gcd(b, a % b);
}

long long Inverse(long long a, long long p)
{
    long long x, y;
    ex_gcd(a, p, x, y);
    x = x + (x < 0 ? p : 0);
    return x;
}

int main()
{
    printf("Input 2 integers: ");
    long long x, y, ans;
    scanf("%lld%lld", &x, &y);
    ans = gcd(x, y);
    printf("GCD is %lld\n\n", ans);

    printf("Input an integer and mod: ");
    scanf("%lld%lld", &x, &y);
    ans = Inverse(x, y);
    printf("Inverse is %lld\n", ans);
}