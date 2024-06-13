#include<iostream>
#include<iomanip>
#include<stdexcept>
#include<windows.h>
#include "Time.h"

using std::invalid_argument;

void tick(Time &t)
{
	while (true)
	{
		t.printStandard();
		if (t.getSecond()==59)
		{
			t.setSecond(0);
			if (t.getMinute()==59)
			{
				t.setMinute(0);
				if (t.getHour()==23)
					t.setHour(0);
				else
					t.setHour(t.getHour()+1);
			}
			else
				t.setMinute(t.getMinute()+1);
		}
		else
			t.setSecond(t.getSecond()+1);
		
		Sleep(1000);
	}
}

Time::Time(int hour,int minute,int second)
{
	setTime(hour,minute,second);
}

void Time::setTime(int h,int m,int s)
{
	if ((h>=0 && h<24) && (m>=0 && m<60) && (s>=0 && s<60))
	{
		hour=h;
		minute=m;
		second=s;
	}
	else
		throw invalid_argument("hour,minute and/or second was out of range");
}

void Time::setHour(int h)
{
	if (h>=0 && h<24)
		hour=h;
	else
		throw invalid_argument("hour must be 0-23");
}

void Time::setMinute(int m)
{
	if (m>=0 && m<60)
		minute=m;
	else
		throw invalid_argument("minute must be 0-59");
}

void Time::setSecond(int s)
{
	if (s>=0 && s<60)
		second=s;
	else
		throw invalid_argument("second must be 0-59");
}

unsigned int Time::getHour() const
{
	return hour;
}

unsigned int Time::getMinute() const
{
	return minute;
}

unsigned int Time::getSecond() const
{
	return second;
}

void Time::printUniversal() const
{
	std::cout<<std::setfill('0')<<std::setw(2)<<hour<<":"
		<<std::setw(2)<<minute<<":"<<std::setw(2)<<second;
}

void Time::printStandard() const
{
	std::cout<<((hour==0 || hour==12)?12:hour%12)<<":"
		<<std::setfill('0')<<std::setw(2)<<minute<<":"
		<<std::setw(2)<<second<<(hour<12?" AM":" PM")<<std::endl;
}
