#include<iostream>
#include<fstream>
#include<string>
#include <sstream>
using namespace std;

class Student
{
private:
    int id;
    string last_name;
    string first_name;
    int grade;

public:
    Student(int inp_id, string inp_last, string inp_first, int inp_grade):id(inp_id), first_name(inp_first), last_name(inp_last), grade(inp_grade){};
};

void display()
{
    ifstream read("2.dat");
	if (!read)
	{
		cout << "wrong";
        exit(EXIT_FAILURE);
	}
    string a;
	while (getline(read, a))
	{
		cout << a << endl;
	}
    read.close();
}

void add_student()
{
    ifstream read("2.dat");
	if (!read)
	{
		cout << "wrong";
        exit(EXIT_FAILURE);
	}
    int A;
    string B, C;
    int D;

    ofstream write("2.dat", ios::app);
    cout << "Recording student's information" << endl;

    bool repeat = 0;
    int a;
    cout << "Enter your id(5 nums): ";
    cin >> a;
    while ((read>>A>>B>>C>>D)&&(!repeat))
    {
        if (A == a)
        {
            repeat = 1;
            cout << "Your've been already recorded" << endl;
        }
    }

    if(!repeat)
    {
        write << a << " ";

        string k;
        cout << "Enter your last name: ";
        cin >> k;
        write << k << " ";

        string s;
        cout << "Enter your first name: ";
        cin >> s;
        write << s << " ";

        int b;
        cout << "Enter your grade: ";
        cin >> b;
        write << b << endl;

        cout << "ok" << endl;

        write.close();
        read.close();
    }
};


void delete_student()
{
    ifstream read("2.dat");
	if (!read)
	{
		cout << "wrong";
        exit(EXIT_FAILURE);
	}
    int A;
    string B, C;
    int D;
    int a;
    bool deleted = 0;
    string new_file;

    cout << "Deleting student's information" << endl;
    cout << "Enter the student's id: ";
    cin >> a;

    while ((read>>A>>B>>C>>D))
    {
        if (A != a)
        {
            stringstream stream;
            string A1, D1;
            stream << A;
            stream >> A1;
            stream.clear();
            stream << D;
            stream >> D1;
            stream.clear();
            new_file = new_file +  A1 + " " + B + " " + C + " " +  D1 + "\n";
        }
        else
        {
            deleted = true;
        }
    }
    read.close();

    if (deleted == 0)
    {
        cout << "No such student" << endl;
    }
    else
    {
        ofstream write("2.dat",ios::ate);
        cout << "re:"  << new_file << endl;
        write << new_file;
        write.close();
    }
}

int main()
{
    add_student();
    display();
    delete_student();
}