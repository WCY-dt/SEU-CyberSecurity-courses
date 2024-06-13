#include <iostream>
#include <windows.h>
using std::cout;
using std::endl;
using std::ostream;
using std::string;

bool bonus(int month)
{
    SYSTEMTIME systm;
    GetLocalTime(&systm);
    return (systm.wMonth == month);
}

class Date
{
public:
    Date(int iyear = 2014, int imonth = 1, int iday = 1) : year(iyear), month(imonth), day(iday) {}

    friend ostream &operator<<(ostream &output, Date k)
    {
        output << k.year << "-" << k.month << "-" << k.day;
        return output;
    }

    int getMonth()
    {
        return month;
    }

protected:
    int year;
    int month;
    int day;
};

class Employee
{
public:
    Employee(string iname = "John Smith", string isecuritynumber = "111-11-1111", Date ibirthdate = Date()) : name(iname), securitynumber(isecuritynumber), birthdate(ibirthdate) {}

    virtual void show()
    {
        cout << " employee: " << name << endl;
        cout << "birthday: " << birthdate << endl;
        cout << "social security number: " << securitynumber << endl;
    }

protected:
    string name;
    Date birthdate;
    string securitynumber;
};

class salariedEmployee : public Employee
{
public:
    salariedEmployee(string iname = "John Smith", string isecuritynumber = "111-11-1111", Date ibirthdate = Date(), double isalary = 100) : salary(isalary)
    {
        name = iname;
        securitynumber = isecuritynumber;
        birthdate = ibirthdate;
    }

    ~salariedEmployee()
    {
        cout << "deleting object of class SalariedEmployee" << endl;
    }

    void show()
    {
        cout << id;
        Employee::show();
        cout << "weekly salary: " << salary << endl;
        cout << (bonus(birthdate.getMonth()) ? "HAPPY BIRTHDAY!\n" : "");
        cout << "earned: $" << (bonus(birthdate.getMonth()) ? salary + 100 : salary) << endl
             << endl;
    }

protected:
    double salary;
    string id = "salaried";
};

class hourlyemployee : public Employee
{
public:
    hourlyemployee(string iname = "John Smith", string isecuritynumber = "111-11-1111", Date ibirthdate = Date(), double iwage = 20, double ihour = 0) : wage(iwage), hour(ihour)
    {
        name = iname;
        securitynumber = isecuritynumber;
        birthdate = ibirthdate;
    }

    ~hourlyemployee()
    {
        cout << "deleting object of class HourlyEmployee" << endl;
    }
    void show()
    {
        cout << id;
        Employee::show();
        cout << "hourly wage: " << wage << "; ";
        cout << "hours worked: " << hour << endl;
        cout << (bonus(birthdate.getMonth()) ? "HAPPY BIRTHDAY!\n" : "");
        cout << "earned: $" << (bonus(birthdate.getMonth()) ? hour * wage + 100 : hour * wage) << endl
             << endl;
    }

protected:
    double wage;
    double hour;
    string id = "salaried";
};

class commissionemployee : public Employee
{
public:
    commissionemployee(string iname = "John Smith", string isecuritynumber = "111-11-1111", Date ibirthdate = Date(), double isale = 20, double icommission = 0) : sale(isale), commission(icommission)
    {
        name = iname;
        securitynumber = isecuritynumber;
        birthdate = ibirthdate;
    }

    ~commissionemployee()
    {
        cout << "deleting object of class CommissionEmployee" << endl;
    }

    void show()
    {
        cout << id;
        Employee::show();
        cout << "gross sales: " << sale << "; ";
        cout << "commisstion rate: " << commission << endl;
        cout << (bonus(birthdate.getMonth()) ? "HAPPY BIRTHDAY!\n" : "");
        cout << "earned: $" << (bonus(birthdate.getMonth()) ? sale * commission + 100 : sale * commission) << endl
             << endl;
    }

protected:
    double sale;
    double commission;
    string id = "commission";
};

class basecommissionemployee : public Employee
{
public:
    basecommissionemployee(string iname = "John Smith", string isecuritynumber = "111-11-1111", Date ibirthdate = Date(), double isale = 20, double icommission = 0, double ibase = 0) : sale(isale), commission(icommission), base(ibase)
    {
        name = iname;
        securitynumber = isecuritynumber;
        birthdate = ibirthdate;
    }

    void show()
    {
        cout << id;
        Employee::show();
        cout << "gross sales: " << sale << "; ";
        cout << "commisstion rate: " << commission << "; ";
        cout << "base salary: " << base << endl;
        cout << (bonus(birthdate.getMonth()) ? "HAPPY BIRTHDAY!\n" : "");
        cout << "earned: $" << (bonus(birthdate.getMonth()) ? sale * commission + base + 100 : sale * commission + base) << endl
             << endl;
    }

protected:
    double sale;
    double commission;
    double base;
    string id = "base-salaried commission";
};

int main()
{
    Employee *employees[4];
    Date a(1944, 15, 6), b(1960, 5, 29), c(1954, 9, 8), d(1965, 3, 2);
    employees[0] = new salariedEmployee("John Smith", "111-11-1111", a, 800);
    employees[1] = new hourlyemployee("Karen Price", "222-22-2222", b, 16.75, 40);
    employees[2] = new commissionemployee("Sue Jones", "333-33-3333", c, 10000, 0.06);
    employees[3] = new basecommissionemployee("Bob Lewis", "444-44-4444", d, 5000, 0.04, 300);
    for (int i = 0; i < 4; i++)
    {
        employees[i]->show();
    }
}