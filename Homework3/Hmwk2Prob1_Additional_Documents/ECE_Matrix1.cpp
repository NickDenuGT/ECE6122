

#include "ECE_Matrix.h"

using namespace std;

ECE_Matrix::ECE_Matrix():
m_rowSize(0),
m_colSize(0),
m_pathToMatrixFile("")
{
    vector<vector<double>> matrix(m_rowSize);
    for(int i = 0; i < matrix.size(); i++)
    {
        matrix[i] = vector<double>(m_colSize, 0);
    }
    
    m_matrix = matrix;
}

ECE_Matrix::ECE_Matrix(ECE_Matrix const &inCopy):
m_rowSize(inCopy.m_rowSize),
m_colSize(inCopy.m_colSize),
m_matrix(inCopy.m_matrix)
{
    // Do Nothing
}

ECE_Matrix::ECE_Matrix(const int squareSize, const double elements):
m_rowSize(squareSize),
m_colSize(squareSize),
m_pathToMatrixFile("")
{
    vector<vector<double>> matrix(squareSize);
    for(int i = 0; i < matrix.size(); i++)
    {
        matrix[i] = vector<double>(squareSize, elements);
    }
    
    m_matrix = matrix;
}

ECE_Matrix::ECE_Matrix(const int rowSize, const int colSize, const double elements):
m_rowSize(rowSize),
m_colSize(colSize),
m_pathToMatrixFile("")
{
    vector<vector<double>> matrix(m_rowSize);
    for(int i = 0; i < matrix.size(); i++)
    {
        matrix[i] = vector<double>(m_colSize, elements);
    }
    
    m_matrix = matrix;
}

ECE_Matrix::ECE_Matrix(const string pathToMatrixFile):
m_pathToMatrixFile(pathToMatrixFile)
{
    fstream matrixFile;
    matrixFile.open(m_pathToMatrixFile);
    matrixFile >> m_rowSize >> m_colSize;
    
    vector<vector<double>> matrix(m_rowSize);
    
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

ECE_Matrix& ECE_Matrix::operator+(ECE_Matrix const &lhs) const
{
    vector<vector<double>> sumMatrix(max(lhs.m_rowSize, this->m_rowSize));
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
    ECE_Matrix returnMatrix;
    returnMatrix.m_rowSize = max(lhs.m_rowSize, this->m_rowSize);
    returnMatrix.m_colSize = max(lhs.m_colSize, this->m_colSize);
    returnMatrix.m_matrix = sumMatrix;
    return returnMatrix;
}

ECE_Matrix& ECE_Matrix::operator+(long const &lhs) const
{
    vector<vector<double>> sumMatrix(this->m_rowSize);
    for(int i = 0; i < sumMatrix.size(); i++)
    {
        sumMatrix[i] = vector<double>(this->m_colSize);
        for(int j = 0; j < sumMatrix[i].size(); j++)
        {
            sumMatrix[i][j] = this->m_matrix[i][j] + lhs;
        }
    }
    ECE_Matrix returnMatrix(this->m_rowSize, this->m_colSize);
    returnMatrix.m_matrix = sumMatrix;
    return returnMatrix;
}
