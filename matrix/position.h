#ifndef ASH_MATRIX_POSITION_H_INCLUDED
#define ASH_MATRIX_POSITION_H_INCLUDED

#include <unordered_map>
#include <stdexcept>

namespace ash::matrix
{
    class position;
}

class ash::matrix::position
{
    private:
        int x;
        int y;
    
    public:
        position() {}
        position(int x, int y)
        {
            if (x < 0 || y < 0)
            {
                throw std::invalid_argument("Position cannot be negative.");
            }
            
            this->x = x;
            this->y = y;
        }

        friend bool operator==(const position& left, const position& right)
        {
            return left.get_x() == right.get_x() && left.get_y() == right.get_y();
        }
        
        int get_x() const
        {
            return x;
        }
        int get_y() const
        {
            return y;
        }
};

template <>
struct std::hash<ash::matrix::position>
{
    std::size_t operator()(const ash::matrix::position& p) const
    {
        int sum = p.get_x() + p.get_y();
        return sum * (sum + 1) / 2 + p.get_y();
    }
};

#endif