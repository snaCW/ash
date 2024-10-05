#ifndef ASH_MATRIX_POSITION_H_INCLUDED
#define ASH_MATRIX_POSITION_H_INCLUDED

#include <unordered_map>

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
        position();
        position(int x, int y);
        
        friend bool operator==(const position& left, const position& right);
        
        const int get_x() const;
        const int get_y() const;
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