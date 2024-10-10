#ifndef ASH_MATRIX_MATRIX_H_INCLUDED
#define ASH_MATRIX_MATRIX_H_INCLUDED

#include <iostream>
#include <stdexcept>
#include "sparse_matrix.h"
#include "position.h"

namespace ash::matrix
{
    template <class T>
    class matrix;
}

template <class T>
class ash::matrix::matrix
{
    protected:
        T* data = new T[1];
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
            delete[] data;
            data = new T[rows * columns];

            int index = 0;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    stream >> data[index++];
                }
            }
        }
    
    // Destructor
        ~matrix()
        {
            delete[] data;
        }

    // Element Access
        T& at(int rowIndex, int columnIndex)
        {
            throw_if_row_out_of_range(rowIndex);
            throw_if_column_out_of_range(columnIndex);
            return element_at(rowIndex, columnIndex);
        }
        const T& at(int rowIndex, int columnIndex) const
        {
            throw_if_row_out_of_range(rowIndex);
            throw_if_column_out_of_range(columnIndex);
            return element_at(rowIndex, columnIndex);
        }
        T* row_at(int rowIndex)
        {
            throw_if_row_out_of_range(rowIndex);

            T* row = new T[columns];
            for (int i = 0; i < columns; i++)
            {
                row[i] = &element_at(rowIndex, i);
            }
            return row;
        }
        const T* row_at(int rowIndex) const
        {
            throw_if_row_out_of_range(rowIndex);

            T* row = new T[columns];
            for (int i = 0; i < columns; i++)
            {
                row[i] = &element_at(rowIndex, i);
            }
            return row;
        }
        T* column_at(int columnIndex)
        {
            throw_if_column_out_of_range(columnIndex);

            T* column = new T[rows];
            for (int i = 0; i < rows; i++)
            {
                column[i] = &element_at(i, columnIndex);
            }
            return column;
        }
        const T* column_at(int columnIndex) const
        {
            throw_if_column_out_of_range(columnIndex);

            T* column = new T[rows];
            for (int i = 0; i < rows; i++)
            {
                column[i] = &element_at(i, columnIndex);
            }
            return column;
        }
        T* get_data()
        {
            return data;
        }
        const T* get_data() const
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
            delete[] data;
            data = new T[1];
            rows = 1;
            columns = 1;
        }
    
    // Type Detection
        bool is_sparse(float sparseRate = 0.5) const
        {
            T defaultValue = T();
            int defaultValueCount = 0;
            for (int i = 0; i < rows * columns; i++)
            {
                if (data[i] == defaultValue)
                {
                    defaultValueCount++;
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
                    spMatrix.add(element_at(i, j), position(i, j));
                }
            }
            return spMatrix;
        }
    
    protected:
    // Protected Methods
        T& element_at(int rowIndex, int columnIndex)
        {
            return data[rowIndex * columns + columnIndex];
        }
        void throw_if_row_out_of_range(int rowIndex)
        {
            if (rowIndex < 0 || rowIndex >= rows)
            {
                throw std::out_of_range("Row access is out of range.");
            }
        }
        void throw_if_column_out_of_range(int columnIndex)
        {
            if (columnIndex < 0 || columnIndex >= columns)
            {
                throw std::out_of_range("Column access is out of range.");
            }
        }
};

#endif