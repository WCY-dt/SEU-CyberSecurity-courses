#include "HugeInteger.h"

#include<iostream>
#include<cstring>
#include<algorithm>

void HugeInteger::output()
{
	for (size_t i=this->integer[0];i>=1;i--)
		std::cout<<this->integer[i];
}



HugeInteger::HugeInteger(int ilongh)
{
	long longh=ilongh;
	memset(integer,0,sizeof(integer));
	while (longh>0)
	{
		integer[0]++;
		integer[integer[0]]=longh%10;
		longh/=10;
	}
}
HugeInteger::HugeInteger(const char *h)
{
	memset(integer,0,sizeof(integer));
	while (h[integer[0]]!='\0')
	{
		for (size_t i=integer[0];i>=1;i--)
			integer[i+1]=integer[i];
		integer[0]++;
		integer[1]=h[integer[0]-1]-'0';
	}
}



bool HugeInteger::isEqualTo(const HugeInteger &h)
{
	if (this->integer[0]!=h.integer[0])
		return false;
	for (size_t i=1;i<=this->integer[0];i++)
		if (this->integer[i]!=h.integer[i])
			return false;
	return true;
}
bool HugeInteger::isNotEqualTo(const HugeInteger &h)
{
	if (this->integer[0]!=h.integer[0])
		return true;
	for (size_t i=1;i<=this->integer[0];i++)
		if (this->integer[i]!=h.integer[i])
			return true;
	return false;
}
	
bool HugeInteger::isLessThan(const HugeInteger &h)
{
	if (this->integer[0]<h.integer[0])
		return true;
	if (this->integer[0]>h.integer[0])
		return false;
	for (size_t i=this->integer[0];i>=1;i--)
	{
		if (this->integer[i]<h.integer[i])
			return true;
		if (this->integer[i]>h.integer[i])
			return false;
	}
	return false;
}
bool HugeInteger::isLessThanOrEqualTo(const HugeInteger &h)
{
	if (this->integer[0]<h.integer[0])
		return true;
	if (this->integer[0]>h.integer[0])
		return false;
	for (size_t i=this->integer[0];i>=1;i--)
	{
		if (this->integer[i]<h.integer[i])
			return true;
		if (this->integer[i]>h.integer[i])
			return false;
	}
	return true;
}
bool HugeInteger::isGreaterThan(const HugeInteger &h)
{
	if (this->integer[0]>h.integer[0])
		return true;
	if (this->integer[0]<h.integer[0])
		return false;
	for (size_t i=this->integer[0];i>=1;i--)
	{
		if (this->integer[i]>h.integer[i])
			return true;
		if (this->integer[i]<h.integer[i])
			return false;
	}
	return false;
}
bool HugeInteger::isGreaterThanOrEqualTo(const HugeInteger &h)
{
	if (this->integer[0]>h.integer[0])
		return true;
	if (this->integer[0]<h.integer[0])
		return false;
	for (size_t i=this->integer[0];i>=1;i--)
	{
		if (this->integer[i]>h.integer[i])
			return true;
		if (this->integer[i]<h.integer[i])
			return false;
	}
	return true;
}


HugeInteger HugeInteger::add(const HugeInteger &h)
{
	HugeInteger temp((int)0);
	for (int i=1;i<=std::max(this->integer[0],h.integer[0]);i++)
	{
		temp.integer[i]+=(this->integer[i]+h.integer[i]);
		temp.integer[i+1]+=(temp.integer[i]/10);
		temp.integer[i]%=10;
	}
	temp.integer[0]=std::max(this->integer[0],h.integer[0])+1;
	while (temp.integer[temp.integer[0]]==0) temp.integer[0]--;
	return temp;
}

HugeInteger HugeInteger::add(int i)
{
	HugeInteger temp((int)i);
	return this->add(temp);
}

HugeInteger HugeInteger::add(const char *c)
{
	HugeInteger temp(c);
	return this->add(temp);
}



HugeInteger HugeInteger::substract(const HugeInteger &h)
{
	HugeInteger temp((int)0);
	for (int i=1;i<=std::max(this->integer[0],h.integer[0]);i++)
	{
		temp.integer[i]+=(this->integer[i]-h.integer[i]);
		if (temp.integer[i]<0)
		{
			temp.integer[i+1]--;
			temp.integer[i]+=10;
		}
	}
	temp.integer[0]=std::max(this->integer[0],h.integer[0])+1;
	while (temp.integer[temp.integer[0]]==0) temp.integer[0]--;
	return temp;
}

HugeInteger HugeInteger::substract(int i)
{
	HugeInteger temp((int)i);
	return this->substract(temp);
}

HugeInteger HugeInteger::substract(const char *c)
{
	HugeInteger temp(c);
	return this->substract(temp);
}


bool HugeInteger::isZero()
{
	return (this->integer[0])==1;
}


	
int HugeInteger::getLength()
{
	if (integer[integer[0]]==-1) return integer[0]-1;
	return integer[0];
}
