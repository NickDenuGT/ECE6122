/*
 * Author: Nicholas Denu
 * Class: ECE6122
 * Last Date Modified: 09/20/19
 * 
 * 
 * Description: This file is meant to be used for all 
 * functions and creating the ECE_Matrix class. This class
 * is designed to create a matrix and to 
 * allow for several mathematical operations
 * to be done on them.
 */

#include "ECE_Matrix.h"

using namespace std;

/*
 * Purpose: This is the empty constructor.
 * This constructor creates an empty matrix
 * with a size of 0
 * 
 * Inputs: None
 * Outputs: None
 */
ECE_Matrix::ECE_Matrix():
m_rowSize(0),
m_colSize(0)
{
    vector<vector<double> > matrix(m_rowSize);
    for(int i = 0; i < matrix.size(); i++)
    {
        matrix[i] = vector<double>(m_colSize, 0);
    }
    
    m_matrix = matrix;
}

/*
 * Purpose: This constructor is meant to create
 * a deep copy of any other ECE_Matrix class
 * 
 * Inputs: inCopy - The existing class that is being
 *                  copied.
 * Outputs: None
 */
ECE_Matrix::ECE_Matrix(ECE_Matrix const &inCopy):
m_rowSize(inCopy.m_rowSize),
m_colSize(inCopy.m_colSize),
m_matrix(inCopy.m_matrix)
{
    // Do Nothing
}

/*
 * Purpose: This constructor initializes the class, creating a square matrix
 * with the incoming size of elements specified by the input
 * 
 * Inputs: squareSize - The number of rows and columns of the matrix in this class
 *         elements - the elements existing in the matrix
 * Outputs: None
 */
ECE_Matrix::ECE_Matrix(const int squareSize, const double elements):
m_rowSize(squareSize),
m_colSize(squareSize)
{
    vector<vector<double> > matrix(squareSize);
    for(int i = 0; i < matrix.size(); i++)
    {
        matrix[i] = vector<double>(squareSize, elements);
    }
    
    m_matrix = matrix;
}

/*
 * Purpose: This constructor initializes the class, creating a matrix
 * with the incoming size in rows and columns of elements specified by the input
 * 
 * Inputs: rowSize - The size of each row
 *         colSize - The size of each column
 *         elements - the elements existing in the matrix
 * Outputs: None
 */
ECE_Matrix::ECE_Matrix(const int rowSize, const int colSize, const double elements):
m_rowSize(rowSize),
m_colSize(colSize)
{
    vector<vector<double> > matrix(m_rowSize);
    for(int i = 0; i < matrix.size(); i++)
    {
        matrix[i] = vector<double>(m_colSize, elements);
    }
    
    m_matrix = matrix;
}

/*
 * Purpose: This constructor initializes the class, creating a matrix
 * with the incoming size in rows and columns of elements specified by the matrix input
 * 
 * Inputs: rowSize - The size of each row
 *         colSize - The size of each column
 *         matrix - The matrix incoming. Not the same element across the entire matrix
 * Outputs: None
 */
ECE_Matrix::ECE_Matrix(const int rowSize, const int colSize, const vector<vector<double> >matrix):
m_rowSize(rowSize),
m_colSize(colSize),
m_matrix(matrix)
{
    // do nothing
}

/*
 * Purpose: This constructor initializes the class, creating a matrix
 * specified by the matrix in the file specified by the input
 * 
 * Inputs: pathToMatrixFile - The path to the file that contains the matrix
 * being brought in
 * Outputs: None
 */
ECE_Matrix::ECE_Matrix(char* pathToMatrixFile):
m_pathToMatrixFile(pathToMatrixFile)
{
    fstream matrixFile(m_pathToMatrixFile, std::fstream::in);
    matrixFile >> m_rowSize >> m_colSize;
    
    vector<vector<double> > matrix(m_rowSize);
    
    for(int i = 0; i < m_rowSize; i++)
    {
        matrix[i] = vector<double>(m_colSize);
        for(int j = 0; j < m_colSize; j++)
        {
            matrixFile >> matrix[i][j];
        }
    }
    
    m_matrix = matrix;
}

/*
 * Purpose: Prints out the matrix in a scientific clear manner.
 * 
 * Inputs: None
 * Outputs: None
 */
void ECE_Matrix::printMatrix()
{
    for(int i = 0; i < m_matrix.size(); i++)
    {
       for(int j = 0; j < m_matrix[i].size(); j++)
       {
           cout << scientific << setw(12) << setprecision(3) << m_matrix[i][j] << "  ";
       }
       cout << "\n";
    }
}

/*
 * Purpose: Transposes the matrix in the class and returns the transposed matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - transposed version of the class' matrix
 */
ECE_Matrix ECE_Matrix::transpose() const 
{
    ECE_Matrix newMatrix(m_colSize, m_rowSize, 0);
    for(int i = 0 ; i < m_rowSize ; i++)
    {
        for(int j = 0 ; j < m_colSize ; j++)
        {
            newMatrix.m_matrix[j][i] = this->m_matrix[i][j];
        }
    }
    return newMatrix;
}

/*
 * Purpose: An overloaded addition operator for adding this class' matrix
 * to another class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Sum of the two matrices
 */
ECE_Matrix ECE_Matrix::operator+(ECE_Matrix const &lhs) const
{
    vector<vector<double> > sumMatrix(max(lhs.m_rowSize, this->m_rowSize));
    for(int i = 0; i < sumMatrix.size(); i++)
    {
        sumMatrix[i] = vector<double>(max(lhs.m_colSize, this->m_colSize));
        for(int j = 0; j < sumMatrix[i].size(); j++)
        {
            if((i < lhs.m_rowSize && i < this->m_rowSize && j < lhs.m_colSize && j < this->m_colSize))
            {
                sumMatrix[i][j] = lhs.m_matrix[i][j] + this->m_matrix[i][j];
            }
            else if((i < lhs.m_rowSize && j < lhs.m_colSize))
            {
                sumMatrix[i][j] = lhs.m_matrix[i][j];
            }
            else if(i < this->m_rowSize && j < this->m_colSize)
            {
                sumMatrix[i][j] = this->m_matrix[i][j];
            }

        }
    }
    ECE_Matrix returnMatrix(max(lhs.m_rowSize, this->m_rowSize), max(lhs.m_colSize, this->m_colSize), sumMatrix);
    return returnMatrix;
}

/*
 * Purpose: An overloaded addition/equals operator for adding another matrix to this
 * class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Sum of the two matrices
 */
ECE_Matrix ECE_Matrix::operator+=(ECE_Matrix const &lhs)
{
    vector<vector<double> > sumMatrix(max(lhs.m_rowSize, this->m_rowSize));
    for(int i = 0; i < sumMatrix.size(); i++)
    {
        sumMatrix[i] = vector<double>(max(lhs.m_colSize, this->m_colSize));
        for(int j = 0; j < sumMatrix[i].size(); j++)
        {
            if((i < lhs.m_rowSize && i < this->m_rowSize && j < lhs.m_colSize && j < this->m_colSize))
            {
                sumMatrix[i][j] = lhs.m_matrix[i][j] + this->m_matrix[i][j];
            }
            else if((i < lhs.m_rowSize && j < lhs.m_colSize))
            {
                sumMatrix[i][j] = lhs.m_matrix[i][j];
            }
            else if(i < this->m_rowSize && j < this->m_colSize)
            {
                sumMatrix[i][j] = this->m_matrix[i][j];
            }

        }
    }
    
    this->m_matrix = sumMatrix;
    this->m_colSize = max(lhs.m_colSize, this->m_colSize);
    this->m_rowSize = max(lhs.m_rowSize, this->m_rowSize);
    
    return *this;
}

/*
 * Purpose: An overloaded addition operator for adding a value to every 
 * element in this class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Sum matrix
 */
ECE_Matrix ECE_Matrix::operator+(long const &lhs) const
{
    vector<vector<double> > sumMatrix(this->m_rowSize);
    for(int i = 0; i < sumMatrix.size(); i++)
    {
        sumMatrix[i] = vector<double>(this->m_colSize);
        for(int j = 0; j < sumMatrix[i].size(); j++)
        {
            sumMatrix[i][j] = this->m_matrix[i][j] + lhs;
        }
    }
    ECE_Matrix returnMatrix(this->m_rowSize, this->m_colSize, sumMatrix);
    return returnMatrix;
}

/*
 * Purpose: An overloaded subtraction operator for subtracting a matrix's elements
 * from every element in this class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Difference matrix
 */
ECE_Matrix ECE_Matrix::operator-(ECE_Matrix const &lhs) const
{
    return(*this + (-1) * lhs);
}

/*
 * Purpose: An overloaded subtraction operator for subtracting a value from every 
 * element in this class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Difference matrix
 */
ECE_Matrix ECE_Matrix::operator-(long const &lhs) const
{
    return(*this + (-1) * lhs);
}

/*
 * Purpose: An overloaded subtraction/equals operator for subtracting a value from every 
 * element in this class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Difference matrix
 */
ECE_Matrix ECE_Matrix::operator-=(ECE_Matrix const &lhs)
{
    return(*this += (-1) * lhs);
}

/*
 * Purpose: An overloaded multiplication operator for multiplying a value to 
 * every element in this class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Multiplicand matrix
 */
ECE_Matrix ECE_Matrix::operator*(long const &lhs) const
{
    vector<vector<double> > sumMatrix(this->m_rowSize);
    for(int i = 0; i < sumMatrix.size(); i++)
    {
        sumMatrix[i] = vector<double>(this->m_colSize);
        for(int j = 0; j < sumMatrix[i].size(); j++)
        {
            sumMatrix[i][j] = this->m_matrix[i][j] * lhs;
        }
    }
    ECE_Matrix returnMatrix(this->m_rowSize, this->m_colSize, sumMatrix);
    return returnMatrix;
}

/*
 * Purpose: An overloaded division operator for dividing a value from 
 * very element in this class' matrix
 * 
 * Inputs: None
 * Outputs: ECE_Matrix - Dividend matrix
 */
ECE_Matrix ECE_Matrix::operator/(long const &lhs) const
{
    if(lhs == 0)
    {
        cout << "You cannot divide by zero you fool.";
        return *this;
    }
    vector<vector<double> > sumMatrix(this->m_rowSize);
    for(int i = 0; i < sumMatrix.size(); i++)
    {
        sumMatrix[i] = vector<double>(this->m_colSize);
        for(int j = 0; j < sumMatrix[i].size(); j++)
        {
            sumMatrix[i][j] = this->m_matrix[i][j] / lhs;
        }
    }
    ECE_Matrix returnMatrix(this->m_rowSize, this->m_colSize, sumMatrix);
    return returnMatrix;
}
