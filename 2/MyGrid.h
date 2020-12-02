/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Sept.28, 2020
Description:
declaration of MyGrid Class
*/
#pragma once
#include <vector>
#include <string>
#include <fstream>

using namespace std;
// Grid Object
// The structure is loaded from certain .txt file
// Can be operated with addition,substraction,increment,etc.
class MyGrid
{
public:
    // Load the grid info from a txt file
    // Return true if the loading succeeded
    // input: string path of the txt file
    bool loadGridFromFile(const string& filename);

    // Turn all elements in the grid into zeros
    void zeroOutGrid();

    // Return the number of rows of the grid
    long getNumRows() const;

    // Return the number of columns of the grid
    long getNumCols() const;
    
    // Constructor of MyGrid class
    // myX and myY are set zeros and element vector is initialized empty
    MyGrid()
    {
        this->myX = 0;
        this->myY = 0;
        vector<vector<long>> init;
        this->gridElements = init;

    };

    // Reload of addition of two MyGrids
    MyGrid operator+(MyGrid const&) const;
    
    // Reload of substraction of two MyGrids
    MyGrid operator-(MyGrid const&) const;

    // Reload of addition of one MyGrid and one long.
    // The long is added to all elements in the grid
    MyGrid operator+(long const&) const;

    // Reload of addition of one long and one MyGrid.
    // The long is added to all elements in the grid
    friend MyGrid operator+(long const& lhs, MyGrid const& rhs);

    // Addition assignment 
    MyGrid& operator+=(MyGrid const&);//

    // Increment
    MyGrid& operator++(); // Prefix//
    MyGrid operator++(int); // Postfix//

    MyGrid& operator--(); // Prefix//
    MyGrid operator--(int); // Postfix//

    // Negation operator
    // Multiplying all elements in the grid with -1
    MyGrid operator-() const;

    // Return true if two MyGrids are equal
    // Return false otherwise
    bool operator==(MyGrid const&) const;

    // Reload of the outstream operator
    friend fstream& operator<<(fstream& os, const MyGrid& gridIn);//

private:
    // two longs storing the left up corner coordinates of the grid
    long myX, myY;
    // 2D vector storing all elements of the grid
    vector<vector<long>> gridElements;
};

