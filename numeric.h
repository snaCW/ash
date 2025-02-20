#ifndef ASH_NUMERIC_H_INCLUDED
#define ASH_NUMERIC_H_INCLUDED

#include <limits>
#include <stdexcept>
#include <typeinfo>

namespace ash
{
    template <class T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type*>
    class numerics;
}

template <class T, typename>
class ash::numerics
{
private:
    T value;
    constexpr static T max_value = std::numeric_limits<T>::max();
    constexpr static T min_value = std::numeric_limits<T>::min();

public:
    numerics() : value(0) {}

    template <class U>
    numerics(U value) noexcept
    {
        if (!convertable(value, this->value))
            throw std::logic_error("Unconvertable Number.");
        
        this->value = value;
    }
    numerics(const numerics &other) noexcept : value(other.value) {}

    numerics &operator=(const numerics &other) noexcept
    {
        this->value = other.value;
        return *this;
    }
    numerics &operator=(T value) noexcept
    {
        this->value = value;
        return *this;
    }

    bool operator==(const numerics &other) const
    {
        return value == other.value;
    }
    bool operator!=(const numerics &other) const
    {
        return value != other.value;
    }
    bool operator>(const numerics &other) const
    {
        return value > other.value;
    }
    bool operator<(const numerics &other) const
    {
        return value < other.value;
    }

    numerics &operator++()
    {
        if (value > 0 && max_value - value < 1)
            throw std::overflow_error("Numeric Overflow.");

        value = value + 1;
        return *this;
    }
    numerics operator++(int)
    {
        if (value > 0 && max_value - value < 1)
            throw std::overflow_error("Numeric Overflow.");

        numerics temp(value);
        value = value + 1;
        return temp;
    }

    numerics operator+(const numerics &other) const
    {
        if (value > 0 && other.value > 0 && max_value - value < other.value)
            throw std::overflow_error("Numeric Overflow.");
        else if (value < 0 && other.value < 0 && value - min_value > other.value)
            throw std::overflow_error("Numeric Overflow.");
        
        return numerics(value + other.value);
    }
    numerics operator-(const numerics &other) const
    {
        if (value > 0 && other.value < 0 && max_value - value < -other.value)
            throw std::overflow_error("Numeric Overflow.");
        else if (value < 0 && other.value > 0 && value - min_value > -other.value)
            throw std::overflow_error("Numeric Overflow.");
        
        return numerics(value - other.value);
    }
    numerics &operator+=(const numerics &other)
    {
        if (value > 0 && other.value > 0 && max_value - value < other.value)
            throw std::overflow_error("Numeric Overflow.");
        else if (value < 0 && other.value < 0 && value - min_value > other.value)
            throw std::overflow_error("Numeric Overflow.");
        
        value += other.value;
        return *this;
    }
    numerics &operator-=(const numerics &other)
    {
        if (value > 0 && other.value < 0 && max_value - value < -other.value)
            throw std::overflow_error("Numeric Overflow.");
        else if (value < 0 && other.value > 0 && value - min_value > -other.value)
            throw std::overflow_error("Numeric Overflow.");
        
        value -= other.value;
        return *this;
    }

    T operator*() const noexcept
    {
        return value;
    }

    template <class U>
    bool convertable(const U &from, const T &to)
    {
        return true;    
    }
};

template <class T, class U>
bool operator==(const T &other, const ash::numerics<U> &obj)
{
    return obj == other;
}

template <class T, class U>
bool operator!=(const T &other, const ash::numerics<U> &obj)
{
    return obj != other;
}

#endif