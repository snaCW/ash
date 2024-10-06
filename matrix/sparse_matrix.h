#ifndef ASH_MATRIX_SPARSEMATRIX_H_INCLUDED
#define ASH_MATRIX_SPARSEMATRIX_H_INCLUDED

#include <unordered_map>
#include <stdexcept>
#include "position.h"

namespace ash::matrix
{
    template <class T>
    class sparse_matrix;
}

template <class T>
class ash::matrix::sparse_matrix
{
    private:
        int rows;
        int columns;
        std::unordered_map<ash::matrix::position, T> matrix;
        T defaultValue = T();
    
    public:
    // Constructors
        sparse_matrix(int rows, int columns)
        {
            this->rows = rows;
            this->columns = columns;
        }
        sparse_matrix(int rows, int columns, std::unordered_map<ash::matrix::position, T>& matrix)
        {
            this->rows = rows;
            this->columns = columns;
            this->matrix = matrix;
        }
        
    // Element Access
        T& at(int x, int y)
        {
            if (x >= rows || y >= columns)
            {
                throw std::invalid_argument("Out of range.");
            }
            
            auto it = matrix.find(position(x, y));
            if (it == matrix.end())
            {
                return defaultValue;
            }
            return it->second;
        }
        const T& at(int x, int y) const
        {
            if (x >= rows || y >= columns)
            {
                throw std::invalid_argument("Out of range.");
            }
            
            auto it = matrix.find(position(x, y));
            if (it == matrix.end())
            {
                return defaultValue;
            }
            return it->second;
        }
    
    // Inner Data
        int rows_count() const
        {
            return rows;
        }
        int columns_count() const
        {
            return columns;
        }

    // Modifiers
        void add(const T& value, const position& p)
        {
            if (p.get_x() >= rows || p.get_y() >= columns)
            {
                throw std::invalid_argument("Out of range.");
            }
            
            if (value != defaultValue)
            {
                matrix[p] = value;
            }
        }
    
    // Convertors
        sparse_matrix transpose() const
        {
            std::unordered_map<ash::matrix::position, T> matrix;
            for (auto it = this->matrix.begin(); it != this->matrix.end(); it++)
            {
                ash::matrix::position newPosition(it->first.get_y(), it->first.get_x());
                matrix[newPosition] = it->second;
            }

            return ash::matrix::sparse_matrix(columns, rows, matrix);
        }
    
    // Non-member
        friend sparse_matrix operator+(const sparse_matrix& a, const sparse_matrix& b)
        {
            if (a.rows != b.rows || a.columns != b.columns)
            {
                throw std::invalid_argument("Usage of operator+ on different sizes of sparse matrix");
            }

            T defaultValue = T();
            std::unordered_map<ash::matrix::position, T> matrix = a.matrix;
            for (auto it = b.matrix.begin(); it != b.matrix.end(); it++)
            {
                matrix[it->first] += it->second;
                if (matrix[it->first] == defaultValue)
                {
                    matrix.erase(it->first);
                }
            }

            return ash::matrix::sparse_matrix(a.rows, a.columns, matrix);
        }
};

#endif