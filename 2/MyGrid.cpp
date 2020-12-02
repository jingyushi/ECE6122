/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Sept.28, 2020
Description:
implementation of MyGrid Class
*/
#include "MyGrid.h"
#include <sstream>
#include <cassert>
#include <iostream>

using namespace std;
// Load the grid info from a txt file
// Return true if the loading succeeded
// input: string path of the txt file
bool MyGrid::loadGridFromFile(const string& filename)
{
	ifstream inFile;
	inFile.open(filename);
	assert(inFile.is_open());
	string firstLine;
	getline(inFile,firstLine);
	istringstream firstLineStream(firstLine);
	firstLineStream>>this->myX;
	firstLineStream>>this->myY;
	string secLine;//Not used
	getline(inFile,secLine);//Still have to call getline to skip the second line
	string line;
	for(int i = 0;getline(inFile,line);i++)
	{
		long currElement;
		istringstream currLine(line);
		vector<long> vCurrLine;
		while(currLine >> currElement)
		{
			vCurrLine.push_back(currElement);
		}
		this->gridElements.push_back(vCurrLine);
		vCurrLine.clear();
		line.clear();		
	}
	inFile.close();
	return true;
}
// Turn all elements in the grid into zeros
void MyGrid::zeroOutGrid()
{
	for(int i=0;i<this->gridElements.size();++i)
	{
		for(int j=0;j<this->gridElements[i].size();++j)
		{
			this->gridElements[i][j]=0;
		}
	}
}
// Return the number of rows of the grid
long MyGrid::getNumRows() const
{
	return long(this->gridElements.size());
}
// Return the number of columns of the grid
long MyGrid::getNumCols() const
{
	return long(this->gridElements[0].size());
}
// Reload of addition of one MyGrid and one long.
// The long is added to all elements in the grid
MyGrid MyGrid::operator+(long const& b) const
{
	MyGrid result;
	result.myX = this->myX;
	result.myY = this->myY;
	for (int i = 0; i < this->gridElements.size(); ++i)
	{	
		vector<long> temp;
		for (int j = 0; j < this->gridElements[i].size(); ++j)
		{
			temp.push_back(this->gridElements[i][j]+b);
		}
		result.gridElements.push_back(temp);
		temp.clear();
	}
	return result;
}
// Reload of addition of two MyGrids
// Both grids are extended into the maximum expansion
// to make two grids the same size
// The upper left corner of the new grid is assigned the new myX and myY
// the addtion is applied after the expansion above.
MyGrid MyGrid::operator+(MyGrid const& b) const
{
	if (!(this->gridElements.size() && this->gridElements[0].size()))
	{
		return b;
	}//this empty
	else if (!(b.gridElements.size() && b.gridElements[0].size()))
	{
		return (*this);
	}//addition empty
	else if (!(this->gridElements.size() && this->gridElements[0].size()) && !(b.gridElements.size() && b.gridElements[0].size()))
	{
		return(*this);//both empty
	}
	MyGrid result;
	int h, w;
	bool isLeft = this->myX < b.myX; //if this is the left one
	bool isUp = this->myY > b.myY; // if this is the upper one
	int ah = (int)this->getNumRows();
	int bh = (int)(b.getNumRows());
	int aw = (int)(this->getNumCols());
	int bw = (int)(b.getNumCols());
	result.myX = isLeft ? this->myX : b.myX;
	result.myY = isUp ? this->myY : b.myY;
	h = (int)(isUp ? this->myY - b.myY + bh : b.myY - this->myY + ah);
	w = (int)(isLeft ? b.myX - this->myX + bw : this->myX - b.myX + aw);
	MyGrid temp1, temp2;
	temp1.gridElements.resize(h);
	temp2.gridElements.resize(h);
	result.gridElements.resize(h);
	for (int i = 0; i < h; ++i)
	{
		result.gridElements[i].resize(w);
		temp1.gridElements[i].resize(w);
		temp2.gridElements[i].resize(w);
	}
	result.zeroOutGrid();
	temp1.zeroOutGrid();
	temp2.zeroOutGrid();
	int offsetx1 = (int)(this->myX - result.myX);
	int offsety1 = (int)(result.myY-this->myY);
	int offsetx2 = (int)(b.myX - result.myX);
	int offsety2 = (int)(result.myY-b.myY);
	for (int i = 0; i < ah; ++i)
	{
		for (int j = 0; j < aw; ++j)
		{
			temp1.gridElements[i + offsety1][j + offsetx1] = this->gridElements[i][j];
		}
	}
	for (int i = 0; i < bh; ++i)
	{
		for (int j = 0; j < bw; ++j)
		{
			temp2.gridElements[i + offsety2][j + offsetx2] = b.gridElements[i][j];
		}
	}
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			result.gridElements[i][j] = temp1.gridElements[i][j] + temp2.gridElements[i][j];
		}
	}
	return result;
}
// Addition assignment 
// Similiar implementation with addition operator
// but returning this pointer instead
MyGrid& MyGrid::operator+=(MyGrid const& b)
{
	if (!(this->gridElements.size() && this->gridElements[0].size()))//this empty
	{
		this->myX = b.myX;
		this->myY = b.myY;
		this->gridElements = b.gridElements;
		return(*this);
	}
	else if (!(b.gridElements.size() && b.gridElements[0].size())) return (*this);//addition empty
	else if (!(this->gridElements.size() && this->gridElements[0].size()) && !(b.gridElements.size() && b.gridElements[0].size()))
		return(*this);//both empty
	int h, w;
	bool isLeft = this->myX < b.myX; //if this is the left one
	bool isUp = this->myY > b.myY; // if this is the upper one
	int ah = int(this->getNumRows());
	int bh = int(b.getNumRows());
	int aw = int(this->getNumCols());
	int bw = int(b.getNumCols());
	long newMyX = isLeft ? this->myX : b.myX;
	long newMyY = isUp ? this->myY : b.myY;
	h = int(isUp ? this->myY - b.myY + bh : b.myY - this->myY + ah);
	w = int(isLeft ? b.myX - this->myX + bw : this->myX - b.myX + aw);
	vector<vector<long>> copiedThisGridElements; // We wanted to return this pointer, so a copy is made here
	copiedThisGridElements = this->gridElements;// so that the original 2d vector can be changed
	MyGrid temp1, temp2;
	temp1.gridElements.resize(h);
	temp2.gridElements.resize(h);
	this->gridElements.resize(h);
	for (int i = 0; i < h; ++i)
	{
		this->gridElements[i].resize(w);
		temp1.gridElements[i].resize(w);
		temp2.gridElements[i].resize(w);
	}
	this->zeroOutGrid();
	temp1.zeroOutGrid();
	temp2.zeroOutGrid();
	int offsetx1 = int(this->myX - newMyX);
	int offsety1 = int(newMyY - this->myX);
	int offsetx2 = int(b.myX - newMyX);
	int offsety2 = int(newMyY - b.myY);
	for (int i = 0; i < ah; ++i)
	{
		for (int j = 0; j < aw; ++j)
		{
			temp1.gridElements[i + offsety1][j + offsetx1] = copiedThisGridElements[i][j];
		}
	}
	for (int i = 0; i < bh; ++i)
	{
		for (int j = 0; j < bw; ++j)
		{
			temp2.gridElements[i + offsety2][j + offsetx2] = b.gridElements[i][j];
		}
	}
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			this->gridElements[i][j] = temp1.gridElements[i][j] + temp2.gridElements[i][j];
		}
	}
	this->myX = newMyX;
	this->myY = newMyY;
	return (*this);
}
// Reload of substraction of two MyGrids
// Easy implementation by adding the negative of the rhs
MyGrid MyGrid::operator-(MyGrid const& b) const
{
	return (*this) + (-(b));
}
// Return true if two MyGrids are equal
// Return false otherwise
bool MyGrid::operator==(MyGrid const& b) const
{
	if (this->myX != b.myX || this->myY != b.myY) return false;
	if (this->gridElements.size() != b.gridElements.size()) return false;
	if ((this->gridElements.size() && b.gridElements.size())
		&& (this->gridElements[0].size() != b.gridElements[0].size())) return false;
	for (int i = 0; i < this->gridElements.size(); ++i)
	{
		for (int j = 0; j < this->gridElements[i].size(); ++j)
		{
			if (this->gridElements[i][j] != b.gridElements[i][j]) return false;
		}
	}
	return true;
}
// Increment
// Prefix//
MyGrid& MyGrid::operator++()
{
	for (int i = 0; i < this->gridElements.size(); ++i)
	{
		for (int j = 0; j < this->gridElements[i].size(); ++j)
		{
			this->gridElements[i][j]++;
		}
	}
	return *this;
}
// Increment
// Postfix//
MyGrid MyGrid::operator++(int)
{
	MyGrid temp = *this;
	++(*this);
	return temp;
}
// Increment
// Prefix//
MyGrid& MyGrid::operator--()
{
	for (int i = 0; i < this->gridElements.size(); ++i)
	{
		for (int j = 0; j < this->gridElements[i].size(); ++j)
		{
			this->gridElements[i][j]--;
		}
	}
	return *this;
}
// Increment
// Postfix//
MyGrid MyGrid::operator--(int)
{
	MyGrid temp = *this;
	--(*this);
	return temp;
}
// Negation operator
// Multiplying all elements in the grid with -1
MyGrid MyGrid::operator-() const
{
	MyGrid result;
	result.myX = this->myX;
	result.myY = this->myY;
	for (int i = 0; i < this->gridElements.size(); ++i)
	{
		vector<long> temp;
		for (int j = 0; j < this->gridElements[i].size(); ++j)
		{
			temp.push_back(-this->gridElements[i][j]);
		}
		result.gridElements.push_back(temp);
		temp.clear();
	}
	return result;
}
// Reload of addition of one long and one MyGrid.
// The long is added to all elements in the grid
MyGrid operator+(long const& a, MyGrid const& b)
{
	MyGrid result;
	result.myX = b.myX;
	result.myY = b.myY;
	for (int i = 0; i < b.gridElements.size(); ++i)
	{
		vector<long> temp;
		for (int j = 0; j < b.gridElements[i].size(); ++j)
		{
			temp.push_back(b.gridElements[i][j] + a);
		}
		result.gridElements.push_back(temp);
		temp.clear();
	}
	return result;
}
// Reload of the outstream operator
fstream& operator<<(fstream& os, const MyGrid& gridIn)
{
	os << gridIn.myX << ' '<< gridIn.myY << ' '<< endl;
	int h = int(gridIn.getNumRows());
	int w = int(gridIn.getNumCols());
	os << h << ' ' << w << ' ' << endl;
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			os << gridIn.gridElements[i][j]<<' ';
		}
		os << endl;
	}
	return os;
}