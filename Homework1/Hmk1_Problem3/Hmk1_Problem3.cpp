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
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>

bool isFibonacci(int);
void populateFibonacciArray(int, int *);
bool permutate(int, int, int *);
void swap(int *, int, int);
bool isSolution(int *, int);
void printSolution(int *, int);

int *fibonacci_array;

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{

    int *table_array, num_knights;
    int tmp_value;
    
    cout << "Enter the number of knights: ";
    cin >> num_knights;
    
    // Allocate size for arrays used in the application
    table_array = (int *) malloc(sizeof(int) * num_knights);
    fibonacci_array = (int *) malloc(sizeof(int) * (num_knights + 1));
    
    populateFibonacciArray(num_knights + 2, fibonacci_array);
     
    // Populate the seating arrangement array
    for(int i = 1; i <= num_knights ; i++)
    {
        table_array[i - 1] = i;
    }
    
    if(! (permutate(num_knights, 0, table_array)))
    {
        printf("There are no solutions for this number of knights.");
    }
    
    free(table_array);
    free(fibonacci_array);
    
    return 0;
}

/*
Description:
 
This function checks whether or not the input value
exists in the fibonacci sequence.

Input: Value - Number being checked to be in the fibonacci sequence
Output: True if value is found in the sequence, false if not
*/
bool isFibonacci(int value)
{
    for(int i = 0 ; fibonacci_array[i] <= value ; i++)
    {
        if(value == fibonacci_array[i])
        {
            return true;
        }
    }
    return false;
}

/*
Description:
 
This function populates a global fibonacci array that is used to check
each adjecent chair sum.

Input: Size - the size of the Fibonacci array to fill
       Fibonacci_array - Pointer to the sequence of fibonacci values that will be populated.
Output: No output
*/
void populateFibonacciArray(int size, int *fibonacci_array)
{
    int low_val = 1, high_val = 1, temp_val;
    fibonacci_array[0] = low_val;
    
    for(int i = 1; i < size; i++)
    {
        fibonacci_array[i] = high_val;
        temp_val = low_val;
        low_val = high_val;
        high_val = temp_val + low_val;
    }
}

/*
Description:
 
This function takes an array of values and finds all permutations of the array 
by calling itself recursively. Once each permutation is formed, the function
checks to see if the permutation is a valid seating formation. If so, the 
function prints out the solution, ends and returns true.

Input: Size - The size of the table_array
       Index - The input that changes with every recursive call to indicate the 
index in which the function will swap
       Table_array - A pointer to the array which will be acted upon to create
permutations of the original table_array.
Output: True if a valid seating arrangement is found, false if not.
*/
bool permutate(int size, int index, int* table_array)
{
    for(int i = 0; i < size - index; i++)
    {
        if(index == (size - 1))
        {
            if(isSolution(table_array, size))
            {
                printSolution(table_array, size);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            swap(table_array, i, (size - index - 1));
            if(permutate(size, index + 1, table_array))
            {
                return true;
            }
            else
            {
                    swap(table_array, (size - index - 1), i);
            }
        }   
    }
    return false;
}

/*
Description:
 
This function checks a sequence of seating arrangements to check if all the 
sums of adjecent seats are in the fibonacci sequence.

Input: Table_array - The proposed seating arrangment array
       Size - The number of knights being seated or integers in the table_array
Output: True if seating arrangment is a valid solution, false if not
*/
bool isSolution(int *table_array, int size)
{
    for(int i = 0; i < size - 1; i++)
    {
        if(! isFibonacci(table_array[i] + table_array[i+1]))
        {
            return false;
        }
        else
        {
            // Do nothing, continue with process
        }
    }
    return true;
}

/*
Description:
 
This function swaps two indices in a given array

Input: Num_array - The array in which two indices will be swapped
       Index1 - The first index in the num_array to be swapped
       Index2 - The second index in the num_array to be swapped
Output: No output
*/
void swap(int* num_array, int index1, int index2)
{
    int temp_num = 0;
    temp_num = num_array[index1];
    num_array[index1] = num_array[index2];
    num_array[index2] = temp_num;
}


/*
Description:
 
This function prints out the correct solution

Input: Table_array - An array that contains the first solution found
       Size - The size of the table_array
Output: No output
*/
void printSolution(int *table_array, int size)
{
    printf("The solution is: K, ");
    for(int i = 0; i < size - 1; i++)
    {
        printf("%d, ", table_array[i]);
    }
    printf("%d.", table_array[size - 1]);
}