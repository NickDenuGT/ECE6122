/*
 * Author: Nicholas Denu
 * Class: ECE6122
 * Last Modified 09/22/2019
 * 
 * Description: This class reads in a file specified by a command line input. 
 * That file contains a matrix of values that will be read in and analyzed. The
 * output of the file will be the largest multiplicand of four adjacent
 * values in the file matrix.
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>


using namespace std;

long multiplyFunc(int, int, int, int);
bool containsZeros(int, int, int, int);

/*
 * Purpose: The main function reads in the command line filename input, reads in 
 * the file that is specified. That matrix is then looked through for the 
 * greatest multiplicand by four adjacent values. That value is returned.
 * 
 * Inputs: argc - The number of command line arguments
 *         argv - The array of command line arguments. This contains the 
 *                matrix file
 * Output: int - Returns 0, as per main functions
 */
int main(int argc, char** argv)
{
    fstream inFile(argv[1]);
    
    int rows, cols;
    
    inFile >> rows >> cols;
    
    vector<vector<int>> inMatrix(rows);
    
    for(int i = 0; i < rows; i++)
    {
        inMatrix[i] = vector<int>(cols);
        for(int j = 0; j < cols; j++)
        {
            inFile >> inMatrix[i][j];
        }
    }
    cout << rows << cols << endl;
    long maxValue = 0;
    long tempLong = 0;
    
    //This nested for loop goes through every index in the array. At every 
    //point there is a check to the three adjacent values as indicated by the 
    //comments above the if statements. Based on the index currently being looked
    //at, you also cannot look in every direction if you are in an edge case. Hence
    //the multiple if statements.
    for(int i0 = 0, i1 = 1, i2 = 2, i3 = 3 ; i0 < rows; i0++, i1++, i2++, i3++)
    {
        for(int j0 = 0, j1 = 1, j2 = 2, j3 = 3 ; j0 < cols; j0++, j1++, j2++, j3++)
        {
            // Check below
            if((i0 < rows - 4) && (!containsZeros(inMatrix[i0][j0], inMatrix[i1][j0], inMatrix[i2][j0], inMatrix[i3][j0]))) 
            {
                tempLong = multiplyFunc(inMatrix[i0][j0], inMatrix[i1][j0], inMatrix[i2][j0], inMatrix[i3][j0]);
                if(tempLong > maxValue)
                {
                    maxValue = tempLong;
                }
            }
            // Check to the right
            if((j0 < cols - 4) && (!containsZeros(inMatrix[i0][j0], inMatrix[i0][j1], inMatrix[i0][j2], inMatrix[i0][j3])))
            {
                tempLong = multiplyFunc(inMatrix[i0][j0], inMatrix[i0][j1], inMatrix[i0][j2], inMatrix[i0][j3]);
                if(tempLong > maxValue)
                {
                    maxValue = tempLong;
                }
            }
            // Check below and to the right
            if((i0 < rows - 4) && (j0 < cols - 4) && (!containsZeros(inMatrix[i0][j0], inMatrix[i1][j1], inMatrix[i2][j2], inMatrix[i3][j3])))
            {
                tempLong = multiplyFunc(inMatrix[i0][j0], inMatrix[i1][j1], inMatrix[i2][j2], inMatrix[i3][j3]);
                if(tempLong > maxValue)
                {
                    maxValue = tempLong;
                }
            }
            // Check below and to the left
            if((i0 >= 3) && (j0 < cols - 4) && (!containsZeros(inMatrix[i0][j0], inMatrix[i0 - 1][j1], inMatrix[i0 - 2][j2], inMatrix[i0 - 3][j3])))
            {
                tempLong = multiplyFunc(inMatrix[i0][j0], inMatrix[i0 - 1][j0 - 1], inMatrix[i0 - 2][j0 - 2], inMatrix[i0 - 3][j0 - 3]);
                if(tempLong > maxValue)
                {
                    maxValue = tempLong;
                }
            }
        }
    }
    
    ofstream outputFile;
    outputFile.open("output2.txt");
    outputFile << maxValue;
    outputFile.close();
    
    return 0;
}

/*
 * Purpose: This function multiplies four ints
 * 
 * Inputs: m1, m2, m3, m4 - All multipliers
 * Output: long - Returns the value of the four inputs multiplied together
 */
long multiplyFunc(int m1, int m2, int m3, int m4)
{
    return (long)m1*m2*m3*m4;
}

/*
 * Purpose: Determine whether or not any of the value inputs are zeros
 * 
 * Inputs: m1, m2, m3, m4 - Values being checked for zeros
 * Outputs: bool - true if there is a zero value, false if not.
 */
bool containsZeros(int m1, int m2, int m3, int m4)
{
    if(m1 == 0)
    {
        return true;
    }
    else if(m2 == 0)
    {
        return true;
    }
    else if(m3 == 0)
    {
        return true;
    }
    else if(m4 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}