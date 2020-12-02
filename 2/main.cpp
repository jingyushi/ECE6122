/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Sept.28, 2020
Description:
test of MyGrid Class
*/
#include <fstream>
#include <iostream>
#include <vector>
#include "MyGrid.h"

using namespace std;

int main(int argc, char* argv[])
{
    fstream outFile("Lab2Output.txt", fstream::out);

    vector<MyGrid> vecGrids;
    MyGrid tmpGrid;

    vecGrids.resize(argc);
    for (int ii = 1; ii < argc; ii++)
    {
        if (!vecGrids[ii-1].loadGridFromFile(argv[ii]))
        {
            outFile << "Failed to load file: " << argv[ii] << endl;
        }

    }
        cout<<0<<endl;
    if (argc >= 3)
    {
        tmpGrid += vecGrids[1];
        outFile << tmpGrid << endl;
	cout<<1<<endl;
        tmpGrid = vecGrids[0] + vecGrids[1];
        outFile << tmpGrid << endl;
	cout<<2<<endl;
        tmpGrid = vecGrids[0] - vecGrids[1];
        outFile << tmpGrid << endl;
	cout<<3<<endl;
        tmpGrid = vecGrids[0] + 10;
        outFile << tmpGrid << endl;
	cout<<4<<endl;
        tmpGrid = 10 + vecGrids[1] + 10;
        outFile << tmpGrid << endl;
	cout<<5<<endl;
        outFile << tmpGrid++ << endl;
        outFile << ++tmpGrid << endl;
	cout<<6<<endl;
        outFile << tmpGrid-- << endl;
        outFile << --tmpGrid << endl;
	cout<<7<<endl;

        outFile << -tmpGrid << endl;
	cout<<8<<endl;
        if (vecGrids[0] == vecGrids[1])
        {
            outFile << "The first two grids are equal" << endl;
        }
        else
        {
            outFile << "The first two grids are not equal" << endl;
        }
	cout<<9<<endl;
        outFile << "The first grid has " << vecGrids[0].getNumRows() << " rows and " << vecGrids[0].getNumCols() << " cols." << endl;
	cout<<10<<endl;
        vecGrids[0].zeroOutGrid();
	cout<<11<<endl;
        outFile << vecGrids[0] << endl;
	cout<<12<<endl;

    }

    outFile.close();

}