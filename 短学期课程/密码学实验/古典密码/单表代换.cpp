#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
using namespace std;

const int numberOfChara = 120; //number of characters

struct prob
{
    char chara; //character
    int freq;   //frequency
} p[26];

char set_freq[] = {'E', 'T', 'A', 'I', 'S', 'O', 'C', 'N', 'R', 'L', 'D', 'H', 'V',
                   'M', 'W', 'F', 'P', 'Y', 'B', 'G', 'U', 'K', 'J', 'X', 'Q', 'Z'}; //sort by the frequency

bool cmp(prob p1, prob p2) //for func sort
{
    return p1.freq > p2.freq;
}

int main()
{
    //initialization
    char c[numberOfChara + 1]; //store the input string
    for (int i = 0; i < 26; i++)
    {
        p[i].chara = i + 'A';
        p[i].freq = 0;
    }

    //input
    for (int i = 1; i <= numberOfChara; i++)
    {
        c[i] = getchar();
        p[c[i] - 'A'].freq++;
    }

    //sort and match
    sort(p, p + 26, cmp);
    printf("Character frequency:\n");
    for (int i=0;i<26;i++)
        printf("%c: %d\n",p[i].chara,p[i].freq);
    printf("\n\nThe plain text is: \n");
    map<char, char> m;
    for (int i = 0; i < 26; i++)
        m[p[i].chara] = set_freq[i];

    //output
    for (int i = 1; i <= numberOfChara; i++)
        cout << m[c[i]];
}