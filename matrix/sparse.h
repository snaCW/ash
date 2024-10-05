#ifndef ASH_MATRIX_SPARSEMATRIX_H_INCLUDED
#define ASH_MATRIX_SPARSEMATRIX_H_INCLUDED

#include <unordered_map>
#include <stdexcept>

namespace ash::matrix
{
    template <class T>
    class sparse_matrix;

    class position;
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
        sparse_matrix(int rows, int columns)
        {
            this->rows = rows;
            this->columns = columns;
        }
        
        void add(const T& value, position p)
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
        
        const T& at(int x, int y)
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
};

#endif