/*
 * Author: Nicholas Denu
 * Class: ECE6122
 * Last Modified 09/22/2019
 * 
 * Description: This header file is used to define the functions in the 
 * ECE_Matrix class. The class defines a matrix and allows multiple operations to be
 * done on that matrix.
 */

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
    ECE_Matrix(const int rowSize, const int colSize, const vector<vector<double>> matrix);
    ECE_Matrix(const string path_to_matrix_file);

    void setRowSize(const int &rowSize) {m_rowSize = rowSize;}
    int getRowSize() {return m_rowSize;}

    void setColSize(const int &colSize) {m_colSize = colSize;}
    int getColSize() {return m_colSize;}
    
    void printMatrix();
    ECE_Matrix transpose() const;
    
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

    
    ECE_Matrix operator+(ECE_Matrix const &) const;
    ECE_Matrix operator+(long const &) const;
    
    friend ECE_Matrix operator+(long const &lhs, ECE_Matrix const &rhs)
    {
        return(rhs + lhs);
    }
    
    ECE_Matrix operator+=(ECE_Matrix const &);
    
    ECE_Matrix operator-(ECE_Matrix const &) const;
    ECE_Matrix operator-=(ECE_Matrix const &);
    
    ECE_Matrix operator-(long const &) const;
    
    friend ECE_Matrix operator-(long const &lhs, ECE_Matrix const &rhs)
    {
        return((-1) * rhs + lhs);
    }
    
    ECE_Matrix operator*(long const &) const;
    
    friend ECE_Matrix operator*(long const &lhs, ECE_Matrix const &rhs)
    {
        return(rhs * lhs);
    }
    
    ECE_Matrix operator/(long const &) const;
    
    friend ECE_Matrix operator/(long const &lhs, ECE_Matrix const &rhs)
    {
        vector<vector<double>> sumMatrix(rhs.m_rowSize);
        for(int i = 0; i < sumMatrix.size(); i++)
        {
            sumMatrix[i] = vector<double>(rhs.m_colSize);
            for(int j = 0; j < sumMatrix[i].size(); j++)
            {
                sumMatrix[i][j] = rhs.m_matrix[i][j] / lhs;
            }
        }
        ECE_Matrix returnMatrix(rhs.m_rowSize, rhs.m_colSize, sumMatrix);
        return returnMatrix;
    }
};
