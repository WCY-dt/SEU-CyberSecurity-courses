/*******************************************************************************
author:王晨阳
description:运用Miller-Rabin素数测试算法，同时运用快速幂算法加快幂运算 
*******************************************************************************/


#include<bits/stdc++.h>
using namespace std;


const long long reversedStudentNumber=10191175;//学号逆序 
long long prime[10]={2,3,5,7,11,13,17,19,23,29};//前30个素数 
//30以内所有素数全部通过可以直接判定学号范围内的数全部无判断错误 


long long qPower(long long base,long long power,long long c)//快速幂
{
    long long ans=1,res=base;
    while(power)
    {
        if(power&1)//power为奇数 
          ans=(ans*res)%c;
        res=(res*res)%c;
        power>>=1;//power/2
    }
    return ans;
}


bool mr(long long number)//Miller-Rabin算法主体
{
    long long a,b,s=0,t=number-1;
    
	//由于本例中判断的数大于2，这边可以省略 
	//    if(number==2)  return true;
	//    if(number<2||!(number&1))  return false;

	//将x-1分解成(2^s)*t
    while(!(t&1))//t为偶数 
    {
        s++;
        t>>=1;
    }
    
    //通过素数表进行测试 
    for(long long i=0;i<10/* && prime[i]<x*/;i++)
    {
        a=qPower(prime[i],t,number);//计算prime[i]^t
        for(long long j=1;j<=s;j++)
        {
            b=(a*a)%number;//计算b的平方 
            if(b==1 && a!=1 && a!=number-1)//二次探测
              return false;
            a=b;
        }
        if(a!=1)//费马小定理
			return false; 
    }
    return true;
}


int main()
{
	long long finded=0;//已经找到的素数个数
	long long temp=reversedStudentNumber;//目前正在检验的数
	if (!(temp&1))//排除偶数 
		temp--;
	while (finded<2)
	{
		if(mr(temp))//如果是素数 
		{
			finded++;
			printf("第%lld大的素数为%lld\n",finded,temp);//输出结果 
		}
    	temp-=2;
    }
    return 0;
}

/*
学号：
57119101

运行结果： 
第1大的素数为10191161
第2大的素数为10191133
*/ 
