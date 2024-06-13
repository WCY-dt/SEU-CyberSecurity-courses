#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
using namespace std;
int main()
{
    ofstream outputFile;
    outputFile.open("IPv4.csv");
    srand(time(0));
    for (int i = 1; i <= 100; i++)
    {
        outputFile << rand() % 256 << '.' << rand() % 256 << '.' << rand() % 256 << '.' << rand() % 256 << ' ';
        char str[50];
        time_t now = time(NULL);
        strftime(str, 50, "%Y-%m-%d %H:%M:%S", localtime(&now));
        outputFile << str << endl;
    }
    outputFile.close();
}