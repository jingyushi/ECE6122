/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.11, 2020
Description:
Implementation of problem 2
*/

#include <iostream>
#include <fstream>
#include <omp.h>
#include <random>
#include <unordered_set>
#include <set>
#include <array>
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>                                                                                                       
#include <getopt.h>

using namespace std;

int main(int argc, char* argv[])
{


	fstream outFile("Lab3Prob2.txt", fstream::out);
	int i; //monte iteration counter
	array<int,2> coordinate = { 0,0 };//coordinate of the ant
	int numSucceeds=0; //number of times of success
	int N; //number of monte trials
	int g; //size of the grid
	int opt;
	while ((opt = getopt(argc, argv, "N:g:")) != -1) {
		switch (opt) {
		case 'N':
			N = atoi(optarg);
			break;
		case 'g':
			g = atoi(optarg);
			break;
		default:
			cout << "please use -N TRIAL_NUMBER -g GRID_SIZE or the inversed way to use this program.";
			exit(-1);
			break;
		}
	}

	bool ifEnd; //if trial reaches the end
	set<array<int,2>> visited;
	visited.insert(coordinate);
	vector<array<int,2>> validMoves;
	int randNum;
	default_random_engine generator;

// main loop for multi-threads
#pragma omp parallel for firstprivate(ifEnd,coordinate,validMoves,visited) shared(numSucceeds) 
	for (i=0; i < N;i++)
	{
		//reset all variables for each loop
		ifEnd = false;
		validMoves.clear();
		coordinate = { 0,0 };
		set<array<int, 2>> visited;//using a set of arrays to store the visited coordinates
		visited.insert(coordinate);

		// the inner loop is for each trial
		for (; !ifEnd;)
		{
			//list all possible moves and store the valid ones into a vector
			array<int, 2> move1 = { coordinate[0] - 1,coordinate[1] };
			array<int, 2> move2 = { coordinate[0] + 1,coordinate[1] };
			array<int, 2> move3 = { coordinate[0],coordinate[1] - 1 };
			array<int, 2> move4 = { coordinate[0],coordinate[1] + 1 };
			if (move1[0] >= 0 && move1[0] < g && move1[1] >= 0 && move1[1] < g && (visited.count(move1) == 0))
				validMoves.push_back(move1);
			if (move2[0] >= 0 && move2[0] < g && move2[1] >= 0 && move2[1] < g && (visited.count(move2) == 0))
				validMoves.push_back(move2);
			if (move3[0] >= 0 && move3[0] < g && move3[1] >= 0 && move3[1] < g && (visited.count(move3) == 0))
				validMoves.push_back(move3);
			if (move4[0] >= 0 && move4[0] < g && move4[1] >= 0 && move4[1] < g && (visited.count(move4) == 0))
				validMoves.push_back(move4);
			if (validMoves.size() == 0) // there is no valid move, jump out of current loop
			{
				ifEnd = true;
				break;
			}
			//only generate a random number up to the size of valid moves
			uniform_int_distribution<int> distribution(0, validMoves.size()-1);
			randNum = distribution(generator);
			coordinate = validMoves[randNum];
			//using a set to store all the visited coordinates
			visited.insert(coordinate);
			validMoves.clear();
			if (coordinate[0] == g - 1 && coordinate[1] == g - 1)
			{
				ifEnd = true;
#pragma omp critical
				{
					// only one thread can modify this variable at a time
					numSucceeds += 1;
				}
				break;
			}
		}

	}
	float result = (long double)numSucceeds/(long double)N;
	//writing the result to the text file
	outFile <<  setprecision(7)  <<fixed<<result;
	outFile.close();
	return 0;
}