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

void oneMoreOne(int, int*);

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{

    int start_num;
    int add_one_cntr = 0;
    int *add_one_cntr_ptr = &add_one_cntr;
    
    cout << "Please enter a starting number: ";
    cin >> start_num;
    
    oneMoreOne(start_num, add_one_cntr_ptr);
    
    cout << "The sequence had " << add_one_cntr << " instances of the number 1 being added";
    
    return 0;
}

/*
Description:
 
This function will recursively call itself. Every time called, there will either
be a 1 added to the current value or the current value will be divided by 7
depending on the current value.

Input: Start_num - The current value that will be divided by 7 or added by 1.
       Add_one_cntr_ptr - This is a pointer to a counter that will count the number
       of ones that will be added before our value approaches 0.
Output: No output
*/
void oneMoreOne(int start_num, int *add_one_cntr_ptr)
{
    if(start_num % 7 == 0)
    {
        oneMoreOne(start_num / 7, add_one_cntr_ptr);
    }
    else if(start_num == 1)
    {
        return;
    }
    else
    {
        (*add_one_cntr_ptr)++;
        oneMoreOne(start_num + 1, add_one_cntr_ptr);
    }
}

