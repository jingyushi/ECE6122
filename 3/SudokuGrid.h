/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.11, 2020
Description:
declaration of SudokuGrid Class
*/
#pragma once
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class SudokuGrid
{
public:
	//Reads a single SudokuGrid object from a fstream file.
	friend fstream& operator>>(fstream& os, SudokuGrid& gridIn);
	//Writes the SudokuGrid object to a file in the same format that is used in reading in the object
	friend fstream& operator<<(fstream& os, const SudokuGrid& gridIn);
	//Solve the Sudoku
	bool solve();

	//Reference:https://www.tutorialspoint.com/sudoku-solver-in-cplusplus
	//check whether num is present in col or not
	bool isPresentInCol(int col, int num);
	//check whether num is present in row or not
	bool isPresentInRow(int row, int num);
	//check whether num is present in 3x3 box or not
	bool isPresentInBox(int boxStartRow, int boxStartCol, int num);
	//get empty location and update row and column
	bool findEmptyPlace(int& row, int& col);
	//when item not found in col, row and current 3x3 box
	bool isValidPlace(int row, int col, int num);
private:
	//Name of the grid
	std::string m_strGridName;
	//Elements of the grid
	unsigned char gridElement[9][9];
};

