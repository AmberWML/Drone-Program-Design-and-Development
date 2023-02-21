#include <iostream>
#include <string>
#include <cmath>
#include "point2.h"
#include <cstdlib>
#include <array>
using namespace std;

	Point2::Point2(){
		x= 0;
		y=0;
	}
	Point2::Point2(float a,float b){
		x= a;
		y = b;

	}
	float Point2::DistanceBetween(Point2 a){
		float dis;
		dis = sqrt(pow(x- a.x,2) + pow(y- a.y,2));
		return dis;
	}
	int Point2::Quadrant(){
		if(x>0 && y> 0){
			int a = 1;
			return a;
		}
		else if(x==0 && y== 0){
			int a = 0;
			return a;
		}
		else if(x>0 && y< 0){
			int a = 4;
			return a;
		}
		else if(x<0 && y> 0){
			int a = 2;
			return a;
		}
		else{
			int a = 3;
			return a;
		}

	}

	void Point2::Print(){
		cout << "x is "<<x <<", and y is "<< y << "."<<endl;

	}