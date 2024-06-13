#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

class Shape
{
    public:
    Shape(string input){
        id = input;
        cout << "Shape " << id << " is created" << endl;
    }
    ~Shape(){
        cout << "destructed" << endl;
    }
    protected:
    string id;
};

class Rectangle: public Shape
{
    public:
    Rectangle(double input_length, double input_width= 0,string id = "Rectangle"):length(input_length),width(input_width),Shape(id){
        cout << "Rectangle is created" << endl;
    }
    ~Rectangle(){
        cout << "Rectangle distructed" << endl;
    }
    int area(){
        return length*width;
    };
    protected:
    int length;
    int width;
};

class circle: public Shape
{
    public:
    int area(){
        return radius*radius*3.14;
    }
    void print(){

    }
    protected:
    int radius;
};

class square: public Rectangle, public circle
{
    protected:
    circle incribe_circle;
};

int main(){
    Rectangle a(1,2);
}