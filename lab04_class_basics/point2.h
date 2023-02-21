# ifndef POINT2_H
# define POINT2_H
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
// This function should “setup” an items array, making
// sure each position in the array contains the string “empty”. to mark it as an empty space.

class Point2{
public:
	Point2();
	Point2(float a,float b);
	int Quadrant();
	float DistanceBetween(Point2 a);
	void Print();
private:
	float x; 
	float y;
};
# endif