/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.11, 2020
Description:
implementation of SudokuGrid Class
The functions to actually solve the sudoku puzzle were obtained from https://www.tutorialspoint.com/sudoku-solver-in-cplusplus
*/
#include "SudokuGrid.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>

using namespace std;

//Reads a single SudokuGrid object from a fstream file.
fstream& operator>>(fstream& os, SudokuGrid& gridIn)
{
	assert(os.is_open());
	getline(os, gridIn.m_strGridName);
	for (int i = 0; i < 9; i++)
	{
		string temp;
		//os.getline(temp,10);
		getline(os, temp);
		for (int j = 0; j < 9; j++)
		{
			gridIn.gridElement[i][j] = (unsigned char) temp[j];
		}
	}
	return os;
}

//Writes the SudokuGrid object to a file in the same format that is used in reading in the object
fstream& operator<<(fstream& os, const SudokuGrid& gridIn)
{
	os << gridIn.m_strGridName << endl;
	for (int i = 0; i < 9; i++)
	{
		//cout <<"i:" <<i << endl;
		for (int j = 0; j < 9; j++)
		{
			os << gridIn.gridElement[i][j];
		}
		os << endl;
	}
	return os;
}

//Reference:https://www.tutorialspoint.com/sudoku-solver-in-cplusplus
	//check whether num is present in col or not
bool SudokuGrid::isPresentInCol(int col, int num)
{
	unsigned char a = (unsigned char)(num+48);
	for (int i = 0; i < 9; i++)
	{
		if (this->gridElement[i][col] == a)
		{
			return true;
		}
	}
	return false;
}
//check whether num is present in row or not
bool SudokuGrid::isPresentInRow(int row, int num)
{
	unsigned char a = (unsigned char)(num + 48);
	for (int i = 0; i < 9; i++)
	{
		if (this->gridElement[row][i] == a)
		{
			return true;
		}
	}
	return false;
}
//check whether num is present in 3x3 box or not
bool SudokuGrid::isPresentInBox(int boxStartRow, int boxStartCol, int num)
{
	unsigned char a = (unsigned char)(num + 48);
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			if (this->gridElement[row + boxStartRow][col + boxStartCol] == a)
				return true;
	return false;
}
//get empty location and update row and column
bool SudokuGrid::findEmptyPlace(int& row, int& col)
{
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			if (this->gridElement[row][col] == '0') //marked with 0 is empty
				return true;
		}
	}
	return false;
}
//when item not found in col, row and current 3x3 box
bool SudokuGrid::isValidPlace(int row, int col, int num)
{
	return !this->isPresentInRow(row, num) && !this->isPresentInCol(col, num) && !this->isPresentInBox(row - row % 3, col - col % 3, num);
}

//Solve the Sudoku
bool SudokuGrid::solve()
{
	int row, col;
	if (!this->findEmptyPlace(row, col))
		return true; //when all places are filled
	for (int num = 1; num <= 9; num++) { //valid numbers are 1 - 9
		if (this->isValidPlace(row, col, num)) { //check validation, if yes, put the number in the grid
			this->gridElement[row][col] = (unsigned char)(num+48);
			if (this->solve()) //recursively go for other rooms in the grid
				return true;
			this->gridElement[row][col] = '0'; //turn to unassigned space when conditions are not satisfied
		}
	}
	return false;
}

