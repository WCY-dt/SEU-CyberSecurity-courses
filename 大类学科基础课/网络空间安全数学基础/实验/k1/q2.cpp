/*******************************************************************************
author:王晨阳
description:运用欧几里得辗转相除法求解gcd和lcm 
*******************************************************************************/


#include<bits/stdc++.h>
using namespace std;

//全部用long long防止溢出 
const long long StudentNumber		 =57119101;//学号 
const long long reversedStudentNumber=10191175;//学号逆序 


long long gcd(long long a,long long b)//递归使用欧几里得辗转相除法求解gcd 
{
    if (a%b==0)
        return b;
    return gcd(b,a%b);
}

long long lcm(long long a,long long b)//用gcd求lcm 
{
    return a*b/gcd(a,b);
}


int main()
{
    printf("最大公约数为%lld\n",gcd(StudentNumber,reversedStudentNumber));
	printf("最小公倍数为%lld\n",lcm(StudentNumber,reversedStudentNumber));
    return 0;
}

/*
学号：
57119101

运行结果： 
最大公约数为1
最小公倍数为582110754133675
*/ 
