/*
 * Author: Nicholas Denu
 * Class: ECE6122
 * Last Modified 09/22/2019
 * 
 * Description: This program reads in a file specified by a command line input.
 * That file contains a number triangle. The program recursively climbs through 
 * the triangle row by row and determines, which track of numbers sums to the 
 * highest value. That value is outputed.
 */

#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

vector<vector<int> > triangle;
int traverse(int, int, int);

/*
 * Purpose: The main function in this program reads in the number triangle. Begins
 * the analysis, and outputs the results of the analysis.
 * 
 * Input: argc - The number of command line inputs
 *        argv - Array of command line inputs
 * 
 * Output: int - 0 always. No error protection here.
 * 
 */
int main(int argc, char** argv)
{
    fstream triFile;
    triFile.open(argv[1]);
    
    int numLevels;
    triFile >> numLevels;
    
    triangle = vector<vector<int> >(numLevels);
    
    for(int i = 0; i < numLevels; i++)
    {
        triangle[i] = vector<int>(i + 1);
        for(int j = 0; j < triangle[i].size(); j++)
        {
            triFile >> triangle[i][j];
        }
    }
    
    triFile.close();
    
    ofstream outputFile;
    outputFile.open("output3.txt");
    
    outputFile << traverse(0, 0, 0);
    
    outputFile.close();
    return 0;
}


/*
 * Purpose: This function is called recursively. It determines which path is beneficial
 * through the number triangle to the next row. 
 * 
 * Input: levelIndex - The current row's index
 *        index - The index in the current row
 *        runningSum - The sum of the preceding values in the number triangle
 *                      path currently being taken
 * 
 * Output: int - The runningSum plus the value of the current location in the 
 *              number triangle
 */
int traverse(int const levelIndex, int const index, int runningSum)
{
    if((levelIndex + 1) == triangle.size())
    {
        //Once getting to the bottom of the triangle, add the value to the 
        // running sum and backtrack
        return runningSum + triangle[levelIndex][index];
    }
    else
    {
        //If there is still a next level to the number triangle, determine
        // which route has a higher sum and return that value.
        return(max(traverse(levelIndex + 1, index, runningSum + triangle[levelIndex][index]), traverse(levelIndex + 1, index + 1, runningSum + triangle[levelIndex][index])));
    }
}

