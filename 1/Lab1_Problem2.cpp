/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Sept.13, 2020
Description:
Lab1_Problem2
*/

#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
	using namespace std;
	unsigned long n,lSum;
	stringstream temp;
	temp << argv[1];
	temp >> n;
	lSum = 0;
	while(n!=1)
	{
		lSum+=n;
		n = n%2==0? n/2:3*n+1;	
	}
	lSum+=1;
	ofstream output;
	output.open("output2.txt");
	output << lSum;
	output.close();
}
