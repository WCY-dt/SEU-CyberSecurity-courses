#include <cmath>
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;
using std::string;

class Account
{
public:
    virtual void calculate(){};

    void debit(double idebit)
    {
        money -= idebit;
        cout << "Name:" << name << endl;
        cout << "Money:" << money << endl
             << endl;
    }

    void credit(double icredit)
    {
        money += icredit;
        cout << "Name:" << name << endl;
        cout << "Money:" << money << endl
             << endl;
    }

protected:
    double money;
    string id;
    string name;
};

class SaveingAccount : public Account
{
public:
    SaveingAccount(string iname, double iinterest = 0, double imoney = 0) : interest(iinterest)
    {
        id = "Saveing";
        money = imoney;
        name = iname;
        cout << "Account created" << endl;
        cout << "Name:" << name << endl;
        cout << "Money:" << money << endl;
        cout << "Type:" << id << endl
             << endl;
    }

    void calculate()
    {
        money *= (1 + interest);
        cout << "Name:" << name << endl;
        cout << "Money:" << money << endl
             << endl;
    }

protected:
    double interest;
};

class CheckingAccount : public Account
{
public:
    CheckingAccount(string iname, double imoney = 0)
    {
        money = imoney;
        id = "Checking";
        name = iname;
        cout << "Account created" << endl;
        cout << "Name:" << name << endl;
        cout << "Money:" << money << endl;
        cout << "Type:" << id << endl
             << endl;
    }

    void calculate()
    {
        cout << "Name:" << name << endl;
        cout << "Money:" << money << endl;
        cout << "Warning:This is a checking account" << endl
             << endl;
    }
};

int main()
{
    Account *accounts[2];
    accounts[0] = new SaveingAccount("John Smith", 0.04, 300);
    accounts[1] = new CheckingAccount("Karen Price", 400);
    accounts[0]->debit(20);
    accounts[1]->debit(40);
    accounts[0]->credit(20);
    accounts[1]->credit(40);
    accounts[0]->calculate();
    accounts[1]->calculate();
}