/*
Author: Nicholas Denu
Class: ECE6122
Last Date Modified: September 04, 2019 

Description:

The purpose of this application is to, given an integer 'n', 
determine a sequence in which all numbers 1-n can be ordered
such that adjacent numbers all sum to a number in the
fibonacci sequence.
*/

#include <cstdlib>
#include <iostream>
#include <math.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{

    int subdivisions;
    double beta, integral_value = 0, step_size;
    
    cout << "Enter the value for the upper limit (beta): ";
    cin >> beta;
    
    cout << "Enter the number of subdivisions to use: ";
    cin >> subdivisions;
    
    step_size = beta / subdivisions;
    
    // This for loop is gone through for every subdivision and the count is kept
    // and added upon itself every time through the for loop.
    for(int i = 0; i < subdivisions; i++)
    {
        integral_value += (4 / (1 + pow((step_size * i + step_size * 0.5), 2))) * step_size;
    }
    
    cout << "The integral evaluates to: " << integral_value << ".";
    
    return 0;
}

