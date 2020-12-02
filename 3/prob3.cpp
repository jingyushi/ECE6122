/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.11, 2020
Description:
Implementation of problem 3
*/
#include <iostream>
#include <fstream>
#include <omp.h>
#include <random>
#include <stdlib.h>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cmath>
#include <unistd.h>                                                                                                       
#include <getopt.h>


using namespace std;

int main(int argc, char* argv[])
{
	fstream outFile("Lab3Prob3.txt", fstream::out);
	int N;
	int i;
	int opt;
    while ((opt = getopt(argc, argv, "N:")) != -1) {
        switch (opt) {
        case 'N':
            N = atoi(optarg);
            break;
        default:        
			cout << "please use -N TRIAL_NUMBER to use this program.";
            exit(-1);
            break;
        }
    }


	double result = numeric_limits<double>::lowest();//initialize the result to be the min value of a double
	default_random_engine generator;
#pragma omp parallel for reduction(max:result) //use max for reduction
	for (i = 0; i < N; i++)
	{
		double temp;
		uniform_real_distribution<double> dst1(0, 1);
		uniform_real_distribution<double> dst2(0, 2);
		uniform_real_distribution<double> dst3(2, 3);
		double x1 = dst1(generator);
		double x2 = dst2(generator);
		double x3 = dst3(generator);//generate different values for each loop
		temp = pow((exp(x1) + x2), 2) + 3 * pow((1 - x3), 2);//compute the value of the function for each loop
		result = max(result,temp);
	}

	//writing the result to the text file
	outFile << setprecision(15) << scientific << result;
	outFile.close();

	return 0;
}