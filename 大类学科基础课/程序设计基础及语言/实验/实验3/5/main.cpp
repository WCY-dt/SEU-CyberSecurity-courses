#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
using std::string;

class Date
{
public:
    Date(int iyear = 2014, int imonth = 1, int iday = 1) : year(iyear), month(imonth), day(iday) {}

    friend ostream &operator<<(ostream &output, Date k)
    {
        output << k.year << "-" << k.month << "-" << k.day;
        return output;
    }

private:
    int year;
    int month;
    int day;
};

class FinalTest : public Date
{
public:
    FinalTest(string ititle = "", Date idate = Date()) : title(ititle), date(idate) {}

    void print()
    {
        cout << "Title: " << title << endl;
        cout << "Test Date: " << date << endl;
    }

    void setDue(Date k)
    {
        date = k;
    }

private:
    string title;
    Date date;
};

int main()
{
    FinalTest item1("C++ Test", Date(2014, 6, 2));
    item1.print();
    FinalTest item2("Java");
    item2.print();
    item2.setDue(Date(2014, 6, 10));
    item2.print();
}