# Ex1

**generate_1.cpp**

```cpp
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
using namespace std;
int main()
{
    ofstream outputFile;
    outputFile.open("IPv4.csv");
    srand(time(0));
    for (int i = 1; i <= 100; i++)
    {
        outputFile << rand() % 256 << '.' << rand() % 256 << '.' << rand() % 256 << '.' << rand() % 256 << ' ';
        char str[50];
        time_t now = time(NULL);
        strftime(str, 50, "%Y-%m-%d %H:%M:%S", localtime(&now));
        outputFile << str << endl;
    }
    outputFile.close();
}
```

**IPv4.csv**

```data
240.139.208.242 2021-05-11 20:01:29
45.76.108.195 2021-05-11 20:01:29
5.32.32.93 2021-05-11 20:01:29
92.233.148.210 2021-05-11 20:01:29
204.112.191.77 2021-05-11 20:01:29
162.87.10.165 2021-05-11 20:01:29
41.80.179.50 2021-05-11 20:01:29
44.141.112.224 2021-05-11 20:01:29
182.120.86.129 2021-05-11 20:01:29
28.163.248.104 2021-05-11 20:01:29
61.2.211.51 2021-05-11 20:01:29
9.88.234.232 2021-05-11 20:01:29
64.239.173.68 2021-05-11 20:01:29
124.140.31.79 2021-05-11 20:01:29
114.162.56.52 2021-05-11 20:01:29
117.198.136.80 2021-05-11 20:01:29
60.99.30.137 2021-05-11 20:01:29
228.175.168.225 2021-05-11 20:01:29
56.221.220.74 2021-05-11 20:01:29
49.149.22.189 2021-05-11 20:01:29
185.161.66.134 2021-05-11 20:01:29
180.233.252.226 2021-05-11 20:01:29
65.161.246.203 2021-05-11 20:01:29
61.193.154.19 2021-05-11 20:01:29
10.181.241.176 2021-05-11 20:01:29
142.86.196.87 2021-05-11 20:01:29
126.26.1.76 2021-05-11 20:01:29
220.135.95.120 2021-05-11 20:01:29
190.238.69.188 2021-05-11 20:01:29
82.85.233.133 2021-05-11 20:01:29
31.181.181.160 2021-05-11 20:01:29
141.102.238.80 2021-05-11 20:01:29
168.215.151.158 2021-05-11 20:01:29
33.130.147.241 2021-05-11 20:01:29
149.31.11.222 2021-05-11 20:01:29
18.24.15.65 2021-05-11 20:01:29
216.62.128.141 2021-05-11 20:01:29
92.186.45.96 2021-05-11 20:01:29
147.70.60.91 2021-05-11 20:01:29
108.157.204.48 2021-05-11 20:01:29
159.47.216.252 2021-05-11 20:01:29
165.26.94.216 2021-05-11 20:01:29
7.86.193.170 2021-05-11 20:01:29
220.47.109.66 2021-05-11 20:01:29
141.249.185.162 2021-05-11 20:01:29
220.255.17.156 2021-05-11 20:01:29
37.188.84.163 2021-05-11 20:01:29
227.78.123.219 2021-05-11 20:01:29
117.39.124.114 2021-05-11 20:01:29
35.6.109.51 2021-05-11 20:01:29
92.37.237.88 2021-05-11 20:01:29
65.180.192.160 2021-05-11 20:01:29
104.135.185.162 2021-05-11 20:01:29
217.12.221.97 2021-05-11 20:01:29
93.127.197.32 2021-05-11 20:01:29
249.97.68.120 2021-05-11 20:01:29
181.38.74.167 2021-05-11 20:01:29
161.46.8.43 2021-05-11 20:01:29
26.246.85.144 2021-05-11 20:01:29
74.144.81.134 2021-05-11 20:01:29
238.80.72.54 2021-05-11 20:01:29
92.201.217.214 2021-05-11 20:01:29
196.247.87.123 2021-05-11 20:01:29
182.190.111.47 2021-05-11 20:01:29
228.147.11.68 2021-05-11 20:01:29
42.122.119.232 2021-05-11 20:01:29
203.48.194.249 2021-05-11 20:01:29
254.169.104.26 2021-05-11 20:01:29
169.189.45.6 2021-05-11 20:01:29
108.30.76.36 2021-05-11 20:01:29
225.141.209.92 2021-05-11 20:01:29
34.77.68.42 2021-05-11 20:01:29
140.216.135.240 2021-05-11 20:01:29
195.210.1.145 2021-05-11 20:01:29
246.59.253.59 2021-05-11 20:01:29
101.232.76.132 2021-05-11 20:01:29
32.52.50.185 2021-05-11 20:01:29
17.243.128.114 2021-05-11 20:01:29
60.167.253.106 2021-05-11 20:01:29
70.247.10.142 2021-05-11 20:01:29
52.92.134.84 2021-05-11 20:01:29
118.30.240.78 2021-05-11 20:01:29
36.127.203.255 2021-05-11 20:01:29
135.53.71.237 2021-05-11 20:01:29
219.30.29.247 2021-05-11 20:01:29
83.48.187.233 2021-05-11 20:01:29
94.175.161.78 2021-05-11 20:01:29
41.162.11.134 2021-05-11 20:01:29
101.135.208.24 2021-05-11 20:01:29
74.70.138.73 2021-05-11 20:01:29
220.100.247.237 2021-05-11 20:01:29
109.121.159.125 2021-05-11 20:01:29
99.228.183.106 2021-05-11 20:01:29
59.189.70.177 2021-05-11 20:01:29
206.11.134.176 2021-05-11 20:01:29
211.55.141.55 2021-05-11 20:01:29
166.193.44.227 2021-05-11 20:01:29
72.50.24.167 2021-05-11 20:01:29
166.82.71.172 2021-05-11 20:01:29
30.154.157.91 2021-05-11 20:01:29
```

**1.cpp**

```cpp
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
int getFirst(string IP)
{
    int tmp = 0;
    int ptr = 0;
    while (IP[ptr] != '.')
    {
        tmp = tmp * 10 + IP[ptr] - '0';
        ptr++;
    }
    return tmp;
}
int main()
{
    ifstream inputFile;
    inputFile.open("IPv4.csv");
    string IP, date, time;
    int ip;
    int A = 0, B = 0, C = 0, D = 0, E = 0;
    while (inputFile >> IP >> date >> time)
    {
        ip = getFirst(IP);
        if (ip >= 0 && ip <= 127)
            A++;
        else if (ip >= 128 && ip <= 191)
            B++;
        else if (ip >= 192 && ip <= 223)
            C++;
        else if (ip >= 224 && ip <= 239)
            D++;
        else if (ip >= 240 && ip <= 247)
            E++;
    }

    cout << "Number of class A address: " << A << endl;
    cout << "Number of class B address: " << B << endl;
    cout << "Number of class C address: " << C << endl;
    cout << "Number of class D address: " << D << endl;
    cout << "Number of class E address: " << E << endl;

    inputFile.close();
}
```

**output**

<img src="E:/tmp/TyporaPic/image-20210511201502399.png" alt="image-20210511201502399" style="zoom:50%;" />

# Ex2

**2.cpp**

```cpp
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
```

**output**

<img src="E:/tmp/TyporaPic/image-20210511211356111.png" alt="image-20210511211356111" style="zoom:50%;" />

