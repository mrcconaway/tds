/**
 * File: shapes.h 
 * Brief: Header file for abstract class Shape and the child classes Circle, Rectangle, and Trapezoid
 * 
 * Author: Ryan Conaway
 * Email: mc321015@ohio.edu
 * 
 * Last modified: 11/6/2020
 */

#include <cmath>
#include <iostream>
using namespace std;
class Shape{


    public:
        Shape() {};
        virtual void input() {
            cout << "Enter Thickness: ";
            cin >> thickness;
            cout << "Enter Cost Per Cubic Unit ";
            cin >> cost_per;
        };
        double cost(){
            return (thickness * cost_per * area() );
        }
        virtual double area() = 0;

    private:
        double cost_per;
        double thickness ;
};


class Circle:public Shape{

    public: 
        Circle(){}
        void input(){
            cout << "Enter radius: ";
            cin >> radius;
            Shape::input();
        }
        double area(){ return (M_PI * radius * radius ); }

    private:
        double radius;

};

class Rectangle:public Shape{

    public:
        Rectangle(){};
        void input(){
            cout << "Enter Length: ";
            cin >> length;
            cout << "\nEnter Width: ";
            cin >> width;
            Shape::input();
        }
        double area(){ return (length * width); }

    private:
        double length, width;
};


class Trapezoid:public Shape{


    public:
        Trapezoid(){};
        void input(){
            cout << "Enter Base1: ";
            cin >> base1;
            cout << "\nEnter Base2: ";
            cin >> base2;
            cout << "\nEnter Height ";
            cin >> height;
            Shape::input();

        }
        double area(){ return( 0.5 * height * (base1 + base2)  );  }

    private:
        double base1, base2, height;

};