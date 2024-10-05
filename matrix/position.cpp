#include "position.h"
#include <stdexcept>

using position = ash::matrix::position;

position::position() {}
position::position(int x, int y)
{
    if (x < 0 || y < 0)
    {
        throw std::invalid_argument("Position cannot be negative.");
    }
    
    this->x = x;
    this->y = y;
}

bool operator==(const position& left, const position& right)
{
    return left.get_x() == right.get_x() && left.get_y() == right.get_y();
}

const int position::get_x() const
{
    return x;
}
const int position::get_y() const
{
    return y;
}