#ifndef ASH_MATRIX_UPPERTRIANGULAR_H_INCLUDED
#define ASH_MATRIX_UPPERTRIANGULAR_H_INCLUDED

#include "matrix.h"
#include <stdexcept>
#include <iostream>

namespace ash::matrix
{
    template <class T>
    class upper_triangular;
}

template <class T>
class ash::matrix::upper_triangular : public ash::matrix::matrix<T>
{
    private:
        T defaultValue = T();
    
    public:
        upper_triangular(int rows, int columns, std::istream& stream)
        {
            if (rows != columns)
            {
                throw std::invalid_argument("No square matrix.");
            }

            this->rows = rows;
            this->columns = columns;
            delete[] this->data;
            this->data = new T[(1 + rows) * (rows) / 2];

            T defaultValue = T();
            int index = 0;
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    T value;
                    stream >> value;

                    if (j < i)
                    {
                        if (value != defaultValue)
                            throw std::invalid_argument("Expected defalut value.");
                        else
                            continue;
                    }
                    this->data[index++] = value;
                }
            }
        }
    
    // Element Access
        T& at(int rowIndex, int columnIndex)
        {
            this->throw_if_row_out_of_range(rowIndex);
            this->throw_if_column_out_of_range(columnIndex);
            return element_at(rowIndex, columnIndex);
        }
        const T& at(int rowIndex, int columnIndex) const
        {
            this->throw_if_row_out_of_range(rowIndex);
            this->throw_if_column_out_of_range(columnIndex);
            return element_at(rowIndex, columnIndex);
        }
        T* row_at(int rowIndex)
        {
            this->throw_if_row_out_of_range(rowIndex);

            T* row = new T[this->rows - rowIndex];
            for (int i = 0; i < this->rows - rowIndex; i++)
            {
                row[i] = &element_at(rowIndex, i);
            }
            return row;
        }
        const T* row_at(int rowIndex) const
        {
            this->throw_if_row_out_of_range(rowIndex);

            T* row = new T[this->rows - rowIndex];
            for (int i = 0; i < this->rows - rowIndex; i++)
            {
                row[i] = &element_at(rowIndex, i);
            }
            return row;
        }
        T* column_at(int columnIndex)
        {
            this->throw_if_column_out_of_range(columnIndex);

            T* column = new T[columnIndex + 1];
            for (int i = 0; i < columnIndex + 1; i++)
            {
                column[i] = &element_at(i, columnIndex);
            }
            return column;
        }
        const T* column_at(int columnIndex) const
        {
            this->throw_if_column_out_of_range(columnIndex);

            T* column = new T[columnIndex + 1];
            for (int i = 0; i < columnIndex + 1; i++)
            {
                column[i] = &element_at(i, columnIndex);
            }
            return column;
        }
    
    private:
    // Private Methods
        T& element_at(int rowIndex, int columnIndex)
        {
            if (columnIndex < rowIndex)
            {
                return defaultValue;
            }
            return this->data[(2 * this->columns - rowIndex + 1) * (rowIndex) / 2 + columnIndex - rowIndex];
        }
};

#endif