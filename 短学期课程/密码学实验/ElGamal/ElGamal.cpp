#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

struct Point
{
    int x, y;
    Point() {}
    Point(int X, int Y)
    {
        x = X;
        y = Y;
    }
};

int Inv[10001];
int k;

int Mod(int a, int p)
{
    return (a % p >= 0 ? a % p : (a % p) + p);
}

void Inverse(int p)
{
    for (int i = 1; i < p; i++)
        for (int j = 1; j < p; j++)
            if ((i * j) % p == 1)
                Inv[i] = j;
}

Point Plus(Point p1, Point p2, int p, int a)
{
    int Lambda;
    Point TmpPoint;
    if (p1.x == p2.x && p1.y == p2.y)
        Lambda = Mod((3 * p1.x * p1.x + a) * Inv[Mod(2 * p1.y, p)], p);
    else
        Lambda = Mod((p2.y - p1.y) * Inv[Mod(p2.x - p1.x, p)], p);
    TmpPoint.x = Mod(Lambda * Lambda - p1.x - p2.x, p);
    TmpPoint.y = Mod(Lambda * (p1.x - TmpPoint.x) - p1.y, p);
    //printf("lambda=%d,x=%d,y=%d\n",Lambda,TmpPoint.x,TmpPoint.y);
    return TmpPoint;
}

Point Minus(Point p1, Point p2, int p, int a)
{
    Point Minus_p2(p2.x, Mod(p - p2.y, p));
    return Plus(p1, Minus_p2, p, a);
}

Point Multiple(int k, Point p1, int p, int a)
{
    Point TmpPoint(p1.x, p1.y);
    for (int i = 1; i < k; i++)
    {
        TmpPoint = Plus(TmpPoint, p1, p, a);
        //printf("(%d,%d)\n",TmpPoint.x,TmpPoint.y);
    }
    return TmpPoint;
}

Point Choose_G(int p, int a, int b)
{
    printf("\nChoose a generator frome below (input in the format of x y):\n");
    vector<Point> G_list;
    for (int i = 0; i < p; i++)
        for (int j = 0; j < p; j++)
            if (((i * i * i + a * i + b) % p) == ((j * j) % p))
                G_list.push_back(Point(i, j));
    for (int i = 0; i < G_list.size(); i++)
        printf("(%d,%d) ", G_list[i].x, G_list[i].y);
    int X, Y;
    printf("\nYou choose ");
    scanf("%d%d", &X, &Y);
    bool flag;
    do
    {
        flag = false;
        for (int i = 0; i < G_list.size(); i++)
        {
            if (G_list[i].x == X && G_list[i].y == Y)
            {
                flag = true;
            }
        }
        if (!flag)
        {
            printf("\nWrong! You should choose among the list above!");
            printf("\nYou choose ");
            scanf("%d%d", &X, &Y);
        }
    } while (!flag);
    //printf("Success1\n");
    return Point(X, Y);
}

Point Implant(int m, int p, int a, int b)
{
    k = rand() % 20 + 30;
    for (int i = 0; i < p; i++)
    {
        int Tmp = Mod(k * m + i, p);
        for (int j = 0; j < p; j++)
            if ((((((Tmp * Tmp) % p) * Tmp) % p + (a * Tmp) % p + b) % p) == ((j * j) % p))
            {
                //printf("Tmp=%d k=%d i=%d j=%d\n",Tmp,k,i,j);
                return Point(Mod(Tmp, p), j);
            }
    }
    return Point(-1, -1);
}

void ElGamal_encode(int &p, int &a, int &b, int &n_A, Point &Cm1, Point &Cm2)
{
    printf("\n------------------------------------------------------------\n");
    printf("ENCODE BEGIN:");
    printf("\n------------------------------------------------------------\n");
    printf("\nInput parameters of ECC(y^2=x^3+ax+b mod p):\n");
    printf("a=");
    scanf("%d", &a);
    printf("b=");
    scanf("%d", &b);
    printf("p=");
    scanf("%d", &p);

    printf("\n");
    Inverse(p);

    //for (int i=1;i<p;i++)
    //printf("Inv[%d]=%d\n",i,Inv[i]);

    Point G = Choose_G(p, a, b);
    //printf("Success2\n");

    printf("\nYou have choosen ECC E_%d(%d,%d) and generator (%d,%d)\n", p, a, b, G.x, G.y);

    printf("\nInput your private key n_A: ");
    scanf("%d", &n_A);

    Point P_A = Multiple(n_A, G, p, a);
    printf("\nYour public key P_A is (%d,%d)\n", P_A.x, P_A.y);

    printf("\nInput your plain text m: ");
    int m;
    scanf("%d", &m);
    Point P_m = Implant(m, p, a, b);
    printf("\nThe message P_m you want to send is (%d,%d)\n", P_m.x, P_m.y);

    int k = rand() % (p - 1) + 1;
    Cm1 = Multiple(k, G, p, a);
    Cm2 = Plus(P_m, Multiple(k, P_A, p, a), p, a);
    printf("Your cipher text is {(%d,%d),(%d,%d)}\n\n", Cm1.x, Cm1.y, Cm2.x, Cm2.y);
}

void ElGamal_decode(int p, int a, int n_A, Point Cm1, Point Cm2)
{
    printf("\n------------------------------------------------------------\n");
    printf("DECODE BEGIN:");
    printf("\n------------------------------------------------------------\n");
    Point P_m = Minus(Cm2, Multiple(n_A, Cm1, p, a), p, a);
    printf("\nThe message P_m is (%d,%d)\n\n", P_m.x, P_m.y);
    //printf("\nYour origin plain text m mod p is %d\n\n",P_m.x/k);
}

int main()
{
    srand(time(0));
    int p, a, b, n_A;
    Point Cm1, Cm2;
    ElGamal_encode(p, a, b, n_A, Cm1, Cm2);
    ElGamal_decode(p, a, n_A, Cm1, Cm2);
}