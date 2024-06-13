#include <fstream>
#include <iostream>
#include <string>
using namespace std;
int getFirst(string IP)
{
    int tmp = 0;
    int ptr = 0;
    while (IP[ptr] != '.')
    {
        tmp = tmp * 10 + IP[ptr] - '0';
        ptr++;
    }
    return tmp;
}
int main()
{
    ifstream inputFile;
    inputFile.open("IPv4.csv");
    string IP, date, time;
    int ip;
    int A = 0, B = 0, C = 0, D = 0, E = 0;
    while (inputFile >> IP >> date >> time)
    {
        ip = getFirst(IP);
        if (ip >= 0 && ip <= 127)
            A++;
        else if (ip >= 128 && ip <= 191)
            B++;
        else if (ip >= 192 && ip <= 223)
            C++;
        else if (ip >= 224 && ip <= 239)
            D++;
        else if (ip >= 240 && ip <= 247)
            E++;
    }

    cout << "Number of class A address: " << A << endl;
    cout << "Number of class B address: " << B << endl;
    cout << "Number of class C address: " << C << endl;
    cout << "Number of class D address: " << D << endl;
    cout << "Number of class E address: " << E << endl;

    inputFile.close();
}