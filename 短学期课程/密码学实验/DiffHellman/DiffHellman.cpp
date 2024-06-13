#include<cstdio>
#include<vector>
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

void Diff(int &p, int &a, int &b)
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

    Point G = Choose_G(p, a, b);

    printf("\nYou have choosen ECC E_%d(%d,%d) and generator (%d,%d)\n", p, a, b, G.x, G.y);

    int n_A,n_B;

    printf("\nInput Alice's private key n_A: ");
    scanf("%d", &n_A);

    Point P_A = Multiple(n_A, G, p, a);
    printf("\nAlice's public key P_A is (%d,%d)\n", P_A.x, P_A.y);

    printf("\nInput Bob's private key n_B: ");
    scanf("%d", &n_B);

    Point P_B = Multiple(n_B, G, p, a);
    printf("\nBob's public key P_B is (%d,%d)\n", P_B.x, P_B.y);

    Point K_A=Multiple(n_A,P_B,p,a);
    printf("\nAlice get the key K=(%d,%d)",K_A.x,K_A.y);

    Point K_B=Multiple(n_B,P_A,p,a);
    printf("\nBob get the key K=(%d,%d)",K_B.x,K_B.y);

    printf("\n\n");
}

int main()
{
    int p,a,b;
    Diff(p,a,b);
}