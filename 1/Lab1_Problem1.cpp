/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Sept.13, 2020
Description:
Lab1_Problem1
*/


#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

bool IsPrime(unsigned long lNum)
{
	unsigned long lFactor;
	for(lFactor=2;lFactor<=sqrt(lNum);lFactor++)
	{
		if(lNum%lFactor==0)
		{
			return false;
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	using namespace std;
	unsigned long n,i;
	stringstream temp;
	temp << argv[1];
	temp >> n;
	unsigned long lSum = 0;
	if(n>=2)
	{
		lSum+=2;
		for(i=3;i<=n;i++)
		{
			if(IsPrime(i)) lSum+=i;
		}
	}
	ofstream output;
	output.open("output1.txt");
	output << lSum;
	output.close();
}


