#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

int width, height, route_count;
void traverse(int, int);

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
int main(int argc, char* argv[])
{    
    
    if(strcmp(argv[1], "-h") == 0)
    {
        height = atoi(argv[2]);
        width = atoi(argv[4]);
    }
    else if(strcmp(argv[1], "-w") == 0)
    {
        height = atoi(argv[4]);
        width = atoi(argv[2]);
    }
    
    route_count = 0;
    
    traverse(0,0);
    
    cout << "Number of Routes: " << route_count << "\n";
    
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
 */
void traverse(int const x_coord, int const y_coord)
{
    if(x_coord < width && y_coord < height)
    {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                traverse(x_coord + 1, y_coord);
            }
            #pragma omp section
            {
                traverse(x_coord, y_coord + 1);
            }
        }
    }
    else if(x_coord < width && y_coord >= height)
    {
        traverse(x_coord + 1, y_coord);
    }
    else if(x_coord >= width && y_coord < height)
    {
        traverse(x_coord, y_coord + 1);
    }
    else
    {
        #pragma omp atomic(update)
        {
            route_count++;
        }
    }
    return;
}
