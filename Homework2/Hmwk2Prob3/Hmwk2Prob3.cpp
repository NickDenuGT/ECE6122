/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: nick
 *
 * Created on September 20, 2019, 2:21 PM
 */

#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

vector<vector<int>> triangle;
int traverse(int, int, int);

/*
 * 
 */
int main(int argc, char** argv)
{
    fstream triFile;
    //triFile.open(argv[1]);
    triFile.open("data_triangle.txt");
    
    int numLevels;
    triFile >> numLevels;
    
    triangle = vector<vector<int>>(numLevels);
    
    for(int i = 0; i < numLevels; i++)
    {
        triangle[i] = vector<int>(i + 1);
        for(int j = 0; j < triangle[i].size(); j++)
        {
            triFile >> triangle[i][j];
        }
    }
    
    cout << traverse(0, 0, 0);
    
    return 0;
}

int traverse(int const levelIndex, int const index, int runningSum)
{
    if((levelIndex + 1) == triangle.size())
    {
        return runningSum + triangle[levelIndex][index];
    }
    else
    {
        return(max(traverse(levelIndex + 1, index, runningSum + triangle[levelIndex][index]), traverse(levelIndex + 1, index + 1, runningSum + triangle[levelIndex][index])));
    }
}

