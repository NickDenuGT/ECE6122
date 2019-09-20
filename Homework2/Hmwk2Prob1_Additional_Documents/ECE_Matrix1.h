#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class ECE_Matrix
{
public:
    ECE_Matrix();
    ECE_Matrix(ECE_Matrix const &);
    ECE_Matrix(const int squareSize, const double elements);
    ECE_Matrix(const int rowSize, const int colSize, const double elements);
    ECE_Matrix(const string path_to_matrix_file);

    void setRowSize(const int &rowSize) {m_rowSize = rowSize;}
    int getRowSize() {return m_rowSize;}

    void setColSize(const int &colSize) {m_colSize = colSize;}
    int getColSize() {return m_colSize;}
    
    void printMatrix();
    
    vector<vector<double>> m_matrix;

    int m_rowSize;
    int m_colSize;
    string m_pathToMatrixFile;
    
    
    friend ostream& operator<<(ostream& os, const ECE_Matrix& ece_mat)
    {
        for(int i = 0; i < ece_mat.m_rowSize; i++)
        {
           for(int j = 0; j < ece_mat.m_colSize; j++)
           {
               os << scientific << setw(12) << setprecision(3) << ece_mat.m_matrix[i][j] << " ";
           }
           os << endl;
        } 
        return os;
    }

    
    ECE_Matrix& operator+(ECE_Matrix const &) const;
    ECE_Matrix& operator+(long const &) const;
    
    friend ECE_Matrix& operator+(long const &lhs, ECE_Matrix const &rhs)
    {
        ECE_Matrix newMatrix(rhs);
        return(newMatrix + lhs);
    }
};

