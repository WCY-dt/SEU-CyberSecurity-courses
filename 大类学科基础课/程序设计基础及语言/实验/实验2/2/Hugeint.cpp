#include "Hugeint.h"
#include<cstring>

std::ostream &operator<<(std::ostream &output,const HugeInt &h)
{
	if (h.integer[h.integer[0]]==-1)
	{
		output<<"-";
		for (size_t i=h.integer[0]-1;i>=1;i--)
			output<<h.integer[i];
	}
	else
		for (size_t i=h.integer[0];i>=1;i--)
			output<<h.integer[i];
	return output;
}

HugeInt::HugeInt(long long ilongh)
{
	long longh=ilongh;
	std::memset(integer,0,sizeof(integer));
	while (longh>0)
	{
		integer[0]++;
		integer[integer[0]]=longh%10;
		longh/=10;
	}
}
HugeInt::HugeInt(const char *h)
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
	

HugeInt& HugeInt::operator=(const HugeInt &h)
{
	for (size_t i=h.integer[0];i>=0;i--)
		this->integer[i]=h.integer[i];
}



bool HugeInt::operator==(const HugeInt &h) const
{
	if (this->integer[0]!=h.integer[0])
		return false;
	for (size_t i=1;i<=this->integer[0];i++)
		if (this->integer[i]!=h.integer[i])
			return false;
	return true;
}
bool HugeInt::operator!=(const HugeInt &h) const
{
	if (this->integer[0]!=h.integer[0])
		return true;
	for (size_t i=1;i<=this->integer[0];i++)
		if (this->integer[i]!=h.integer[i])
			return true;
	return false;
}
	
bool HugeInt::operator< (const HugeInt &h) const
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
bool HugeInt::operator<=(const HugeInt &h) const
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
bool HugeInt::operator> (const HugeInt &h) const
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
bool HugeInt::operator>=(const HugeInt &h) const
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


HugeInt HugeInt::operator+(const HugeInt &h) const
{
	HugeInt temp((long long)0);
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
HugeInt HugeInt::operator-(const HugeInt &h) const
{
	HugeInt temp((long long)0);
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
HugeInt HugeInt::operator*(const HugeInt &h) const
{
	HugeInt temp((long long)0);
	for (int i=1;i<=this->integer[0];i++)
		for (int j=1;j<=h.integer[0];j++)
			temp.integer[i+j-1]+=(this->integer[i]*h.integer[j]);
	for (int i=1;i<=this->integer[0]+h.integer[0]+1;i++)
	{
		temp.integer[i+1]+=(temp.integer[i]/10);
		temp.integer[i]%=10;
	}
	temp.integer[0]=this->integer[0]+h.integer[0]+1;
	while (temp.integer[temp.integer[0]]==0) temp.integer[0]--;
	return temp;
}
HugeInt HugeInt::operator/(const HugeInt &h) const
{
	HugeInt temp((long long)1564);
	return temp;
}
	
int HugeInt::getLength() const
{
	if (integer[integer[0]]==-1) return integer[0]-1;
	return integer[0];
}
