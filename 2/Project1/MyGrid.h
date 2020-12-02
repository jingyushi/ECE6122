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

class MyGrid
{

public:
    bool loadGridFromFile(const string& filename);//
    void zeroOutGrid();//
    long getNumRows() const;//
    long getNumCols() const;//
    MyGrid(){};

    MyGrid operator+(MyGrid const&) const;//
    MyGrid operator-(MyGrid const&) const;//

    MyGrid operator+(long const&) const;//

    friend MyGrid operator+(long const& lhs, MyGrid const& rhs);//

    // Addition assignment 
    MyGrid& operator+=(MyGrid const&);//
    // Increment
    MyGrid& operator++(); // Prefix//
    MyGrid operator++(int); // Postfix//

    MyGrid& operator--(); // Prefix//
    MyGrid operator--(int); // Postfix//

    MyGrid operator-() const;//

    bool operator==(MyGrid const&) const;//

    friend fstream& operator<<(fstream& os, const MyGrid& gridIn);//

private:
    long myX, myY;
    vector<vector<long>> gridElements;
};

