/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Sept.13, 2020
Description:
Lab1_Problem4
*/

#include <cmath>
#include <fstream>
int main(int argc, char* argv[])
{
	using namespace std;
	double x1 = 0.0, y1= 10.1, x2 = 1.4, y2 = -9.6;//starting point and first impact
	int lCount = 0;//count of reflections
	double slope1 = 0.0, slope2 = 0.0, slope3 = 0.0;// slopes of in, normal and out lines
	double intercept = 0.0; //intercept of out line
	double a,b,c; //solving quadratic equation
	double root1 = 0.0, root2 =0.0; // roots of the quadratic equation for intersection
	while(x2>0.01||x2<-0.01||y2<0)
	//any point on the upper ellipse with x between -0,01 and 0.01 will be the last reflection point 
	{
		slope1 = (y2-y1)/(x2 - x1); // in slope
		slope2 = -4*x2/y2;//tangent slope
		slope3 = (slope2-(slope1-slope2)/(1+slope1*slope2))/
			(1+slope2*(slope1-slope2)/(1+slope1*slope2));//out slope
		intercept = y2-slope3*x2;//intercept
		//solving the quadratic equation
		a=4+slope3*slope3;
		b=2*slope3*intercept;
		c=intercept*intercept-100;
		root1 = (-b+sqrt(b*b-4*a*c))/2/a;
		root2 = (-b-sqrt(b*b-4*a*c))/2/a;
		x1 = x2;
		y1 = y2;
		//further root represents the next reflecting point
		x2 = fabs(root1-x2)>fabs(root2-x2)? root1:root2;
		y2 = slope3*x2+intercept;
		lCount++;
	}
	ofstream output;
	output.open("output4.txt");
	output << lCount;
	output.close();
}
