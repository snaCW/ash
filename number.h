#ifndef ASH_NUMBER_H_INCLUDED
#define ASH_NUMBER_H_INCLUDED

#include <vector>
#include <string>

namespace ash
{
    enum sign
    {
        _signed, _unsigned
    };

    template <ash::sign sign = ash::sign::_signed>
    class number;
}

template <ash::sign sign>
class ash::number
{
private:
    std::vector<bool> integer_part;
    std::vector<bool> fractional_part;
public:
    number();
    number(size_t integer_bits);
    number(size_t integer_bits, size_t fractional_bits);
    number(const number &other);

    size_t integer_bits() const;
    size_t fractional_bits() const;
    size_t size() const;
    std::string to_string() const; // Work on it!
    std::string to_binary_string() const;
    
    static number<_signed> convert(bool value);
    static number<_signed> convert(short value);
    static number<_signed> convert(char value);
    static number<_signed> convert(int value);
    static number<_signed> convert(long long value);
    static number<_signed> convert(float value);
    static number<_signed> convert(double value);
    static number<_signed> convert(long double value);
    
    static number<_unsigned> convert(unsigned short value);
    static number<_unsigned> convert(unsigned char value);
    static number<_unsigned> convert(unsigned int value);
    static number<_unsigned> convert(unsigned long long value);

    static void to_twos_compliment(number &value);
    static number<_signed> get_twos_compliment(const number<_signed> &value);

    number operator+(const number &other) const;
    void operator++();
};

/** @brief The default constructor that builds a number exactly equal to 0
 * with
 * 
 * - 32 bits for the integer part and
 * 
 * - 0 bits for the fractional part.
 */
template <ash::sign sign>
ash::number<sign>::number()
{
    integer_part = std::vector<bool>(32, false);
}

/** @brief This constructor builds a number exactly equal to 0
 * with
 * 
 * - 0 bits for the fractional part.
 * 
 * @param integer_bits The number of bits that the integer part will have.
 */
template <ash::sign sign>
ash::number<sign>::number(size_t integer_bits)
{
    integer_part = std::vector<bool>(integer_bits, false);
}

/** @brief This constructor builds a number exactly equal to 0.
 * 
 * @param integer_bits The number of bits that the integer part will have.
 * @param fractional_bits The number of bits that the fractional part will have.
 */
template <ash::sign sign>
ash::number<sign>::number(size_t integer_bits, size_t fractional_bits)
{
    integer_part = std::vector<bool>(integer_bits, false);
    fractional_part = std::vector<bool>(fractional_bits, false);
}

/** @brief This constructor builds a number from another number.
 * 
 * @param other Another number to copy from.
 * 
 * This constructor copies all the bits of the other number.
 */
template <ash::sign sign>
ash::number<sign>::number(const number &other)
{
    integer_part = std::vector<bool>(other.integer_part);
    fractional_part = std::vector<bool>(other.fractional_part);
}

/** @brief This method returns the number of bits that the integer part uses.
 */
template <ash::sign sign>
size_t ash::number<sign>::integer_bits() const
{
    return integer_part.size();
}

/** @brief This method returns the number of bits that the fractional part uses.
 */
template <ash::sign sign>
size_t ash::number<sign>::fractional_bits() const
{
    return fractional_part.size();
}

/** @brief This method returns the total number of bits that are in use.
 */
template <ash::sign sign>
size_t ash::number<sign>::size() const
{
    return integer_part.size() + fractional_part.size();
}

/** @brief This method returns a string representing the current instance. 
 */
template <ash::sign sign>
std::string ash::number<sign>::to_string() const
{
    
}

/** @brief This method returns a binary string representing the current instance. 
 */
template <ash::sign sign>
std::string ash::number<sign>::to_binary_string() const
{
    std::string result;

    bool leading_zero = true;
    size_t bit_index = integer_bits();
    // The reason we don't use `for` loop is that `size_t` cannot fall behind `0`, so we can't check for `bit_index >= 0`
    while (bit_index != 0)
    {
        --bit_index;

        if (integer_part[bit_index] != 0)
            leading_zero = false;
        if (integer_part[bit_index] == 0 && leading_zero)
            continue;
        result += '0' + integer_part[bit_index];
    }

    if (result.empty())
        result += '0';
    
    if (!fractional_part.empty())
    {
        result += '.';
        size_t frac_size = fractional_bits();
        for (bit_index = 0; bit_index < frac_size; ++bit_index)
            result += '0' + fractional_part[bit_index];
    }

    return result;
}

/** @brief This method returns a number built by the given argument.
 * 
 * @param value The given argument to built the number based on.
 * 
 * The result size of the number is only based on the runtime size of the argument,
 * extracted from `sizeof()` method.
 */
template <ash::sign sign>
ash::number<ash::_signed> ash::number<sign>::convert(int value)
{
    size_t size = sizeof(value) * 8;
    bool negetive = value < 0;
    if (negetive)
        value = -value;
    
    number<_signed> result(size);
    for (int bit = 0; bit < size; ++bit)
    {
        result.integer_part[bit] = value & 1;
        value = value >> 1;
    }

    if (negetive)
        to_twos_compliment(result);
    return result;
}

/** @brief This method changes the argument to its "Two's Compliment".
 * 
 * @param value The number to change, passed by reference.
 */
template <ash::sign sign>
void ash::number<sign>::to_twos_compliment(number &value)
{
    size_t bit_index = 0;
    size_t size = value.integer_bits();
    while (bit_index < size && value.integer_part[bit_index] == false)
    {
        ++bit_index;
    }
    
    while (bit_index < size)
    {
        bool bit = value.integer_part[bit_index];
        value.integer_part[bit_index] = !bit;
        ++bit_index;
    }
}

/** @brief This method returns the "Two's Compliment" of the argument.
 * 
 * @param value The number to get its "Two's Compliment".
 */
template <ash::sign sign>
ash::number<ash::_signed> ash::number<sign>::get_twos_compliment(const number<ash::_signed> &value)
{
    number<_signed> temp(value);
    to_twos_compliment(temp);
    return temp;
}

template <ash::sign sign>
ash::number<sign> ash::number<sign>::operator+(const number &other) const
{
    size_t size = std::max(integer_part.size(), other.integer_part.size());
    number sum(size);
    
    size_t bit_index = 0;
    bool carry = false;
    bool a = false;
    bool b = false;
    while (bit_index < integer_part.size() && bit_index < other.integer_part.size())
    {
        a = integer_part[bit_index];
        b = other.integer_part[bit_index];

        sum.integer_part[bit_index] = a xor b xor carry;
        carry = (a && b) || (a && carry) || (b && carry);

        ++bit_index;
    }

    if (bit_index >= integer_part.size())
        a = false;
    if (bit_index >= other.integer_part.size())
        b = false;
    
    while (bit_index < integer_part.size())
    {
        a = integer_part[bit_index];

        sum.integer_part[bit_index] = a xor b xor carry;
        carry = (a && b) || (a && carry) || (b && carry);

        ++bit_index;
    }
    while (bit_index < other.integer_part.size())
    {
        b = other.integer_part[bit_index];

        sum.integer_part[bit_index] = a xor b xor carry;
        carry = (a && b) || (a && carry) || (b && carry);

        ++bit_index;
    }

    if (carry)
        sum.integer_part.push_back(true);
    
    return sum;
}

template <ash::sign sign>
void ash::number<sign>::operator++()
{
    bool carry = integer_part[0];
    integer_part[0] = integer_part[0] xor true;

    size_t bit_index = 1;
    bool a = false;
    while (carry && bit_index < integer_part.size())
    {
        a = integer_part[bit_index];
        integer_part[bit_index] = a xor carry;
        carry = a & carry;

        ++bit_index;
    }

    if (carry)
        integer_part.push_back(true);
}

#endif