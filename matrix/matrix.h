#ifndef ASH_MATRIX_MATRIX_H_INCLUDED
#define ASH_MATRIX_MATRIX_H_INCLUDED

#include <iostream>
#include <stdexcept>
#include "sparse.h"
#include "position.h"

namespace ash::matrix
{
    template <class T>
    class matrix;
}

template <class T>
class ash::matrix::matrix
{
    private:
        T** data = new T*[1];
        int rows;
        int columns;
    
    public:
    // Constructors
        matrix()
        {
            rows = 1;
            columns = 1;
        }
        matrix(const matrix& other)
        {
            data = other.data;
            rows = other.rows;
            columns = other.columns;
        }
        matrix(int rows, int columns, std::istream& stream)
        {
            this->rows = rows;
            this->columns = columns;
            delete data;
            T** data = new T*[rows];
            for (int i = 0; i < rows; i++)
            {
                data[i] = new T[columns];
                for (int j = 0; j < columns; j++)
                {
                    stream >> data[i][j];
                }
            }
        }
    
    // Destructor
        ~matrix()
        {
            delete *data;
            delete data;
        }

    // Element Access
        T& at(int rowIndex, int columnIndex)
        {
            if (rowIndex < 0 || rowIndex >= rows)
            {
                throw std::out_of_range("Access out of range.");
            }
            if (columnIndex < 0 || columnIndex >= columns)
            {
                throw std::out_of_range("Access out of range.");
            }
            return data[rowIndex][columnIndex];
        }
        const T& at(int rowIndex, int columnIndex) const
        {
            if (rowIndex < 0 || rowIndex >= rows)
            {
                throw std::out_of_range("Access out of range.");
            }
            if (columnIndex < 0 || columnIndex >= columns)
            {
                throw std::out_of_range("Access out of range.");
            }
            return data[rowIndex][columnIndex];
        }
        T* row_at(int index)
        {
            if (index < 0 || index >= rows)
            {
                throw std::out_of_range("Access out of range.");
            }
            return data[index];
        }
        const T* row_at(int index) const
        {
            if (index < 0 || index >= rows)
            {
                throw std::out_of_range("Access out of range.");
            }
            return data[index];
        }
        T* column_at(int index)
        {
            if (index < 0 || index >= columns)
            {
                throw std::out_of_range("Access out of range.");
            }

            T* columnData = new T[rows];
            for (int i = 0; i < rows; i++)
            {
                columnData[i] = data[i][index];
            }
            return columnData;
        }
        const T* column_at(int index) const
        {
            if (index < 0 || index >= columns)
            {
                throw std::out_of_range("Access out of range.");
            }

            T* columnData = new T[rows];
            for (int i = 0; i < rows; i++)
            {
                columnData[i] = data[i][index];
            }
            return columnData;
        }
        T* operator[](int index)
        {
            if (index < 0 || index >= rows)
            {
                throw std::out_of_range("Access out of range.");
            }
            return data[index];
        }
        const T* operator[](int index) const
        {
            if (index < 0 || index >= rows)
            {
                throw std::out_of_range("Access out of range.");
            }
            return data[index];
        }
        T** get_data()
        {
            return data;
        }
        const T** get_data() const
        {
            return data;
        }

    // Inner data
        int row_count() const
        {
            return rows;
        }
        int column_count() const
        {
            return columns;
        }
    
    // Modifiers
        void clear()
        {
            delete data;
            data = new T*[1];
            rows = 1;
            columns = 1;
        }
    
    // Type Detection
        bool is_sparse(float sparseRate = 0.5) const
        {
            T defaultValue = T();
            int defaultValueCount = 0;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    if (data[i][j] == defaultValue)
                    {
                        defaultValueCount++;
                    }
                }
            }

            if (defaultValueCount >= rows * columns * sparseRate)
            {
                return true;
            }
            return false;
        }
    
    // Convert Type
        ash::matrix::sparse_matrix<T> to_sparse()
        {
            ash::matrix::sparse_matrix<T> spMatrix(rows, columns);

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    spMatrix.add(data[i][j], position(i, j));
                }
            }
            return spMatrix;
        }
        
};

#endif