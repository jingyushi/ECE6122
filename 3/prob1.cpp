/*
Author: Jingyu Shi
Class: ECE6122
Last Date Modified: Oct.11, 2020
Description:
main function of Problem 1
*/

#include <fstream>
#include <iostream>
#include <vector>
#include "SudokuGrid.h"
#include <thread>
#include <mutex>

using namespace std;

//The following are the global variables
mutex outFileMutex;
mutex inFileMutex;
fstream outFile("Lab3Prob1.txt", fstream::out);
fstream inFile;

//This is the function called by each of the thread to read and solve a sudoku puzzle and then save the answer to the output
void solveSudokuPuzzles()
{
    //cout << 1111;
    SudokuGrid sudoku;
    while (inFile.good())
    {
        inFileMutex.lock();
        inFile >> sudoku;
        inFileMutex.unlock();
        sudoku.solve();
        outFileMutex.lock();
        outFile << sudoku;
        outFileMutex.unlock();
    }
};

//This function is only for debugging
void dummy()
{
    int a = 0;
    a = a + 1;
    std::thread::id tid = std::this_thread::get_id();
    cout << tid << endl;
}

//In the main function, threads are created accordingly and assigned job in parallel
int main(int argc, char* argv[])
{
    int numThreads = thread::hardware_concurrency() - 1;
    //numThreads = 1;
    inFile.open(argv[1]);
    //inFile.open("input_sudoku.txt");
    vector<thread> threads(numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        threads[i] = thread(solveSudokuPuzzles);
    }
    for (auto& th : threads) th.join();

    
    inFile.close();
    outFile.close();
    return 0;
}