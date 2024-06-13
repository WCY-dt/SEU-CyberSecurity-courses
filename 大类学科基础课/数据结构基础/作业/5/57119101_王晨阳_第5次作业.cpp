#include<bits/stdc++.h>
using namespace std;

void getNext(string str,int *next)
{
    next[0]=-1;
    int j=0,k=-1;
	while (j<str.length()-1)
	{
		if (k<0 || str[j]==str[k]) next[++j]=++k;
        else k=next[k];
	}
}

bool KMP(string str1,string str2)
{
    int i=0,j=0,str2length=str2.length(),next[str2length+5];
    bool finded=false;
	getNext(str2,next);
    while (i<str1.length() && j<str2length)
    {
        if (j==-1 || str1[i]==str2[j]) i++,j++;
        else j=next[j];
        if (j==str2length)
		{
			if (!finded) printf("\n找到了！\n可以从模式串的\n");
			finded=true;
			printf("第%d个字符\n",i-j+1);
			i=i-j+1;
			j=0;
		}
    }
    if (finded) printf("开始匹配\n");
    return finded;
}

int main()
{
	string str1,str2;
	printf("模式串为: ");
	cin>>str1;
	printf("匹配串为: ");
	cin>>str2;
	if (!KMP(str1,str2)) printf("\n嘤嘤嘤，没有找到哦\n");
}
