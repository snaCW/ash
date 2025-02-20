#ifndef ASH_RANGE_H_INCLUDED
#define ASH_RANGE_H_INCLUDED

#include <stdexcept>
#include <cmath>

namespace ash
{
    template <class T>
    class range;
}

/** Example usage:
 * 
 * // Create a range from 0 to 10 (exclusive) with step 2
 * ash::range<int> r(0, 10, 2);
 * for (auto i : r)
 *     std::cout << i << " ";  // Outputs: 0 2 4 6 8
 * 
 * // Create a range from 10 to 0 (exclusive) with step -2
 * ash::range<int> r_desc(10, 0, -2);
 * for (auto i : r_desc)
 *     std::cout << i << " ";  // Outputs: 10 8 6 4 2
 */


/** @brief A class that offers the functionality for a ranged based iteration.
 * 
 * @tparam T    Type of `start`, `end`, and `step`.
 * 
 * The default values for `start` is `0`, `end` is `0`, and `step` is `1` if not provided by the user.
 * 
 * If `step` is positive, the refered range is [`start`, `end`) where `start` is inclusive and `end` is exclusive.
 * 
 * If `step` is negative, the refered range is (`end`, `start`] where `start` is inclusive and `end` is exclusive.
 * 
 * If the template argument is not provided, the program uses the default type (that is `int`).
 * 
 * `end` variable is defined as `_end` to avoid the ambiguity with `iterator end()` method.
 */
template <class T = int>
class ash::range
{
private:
    T start = 0;
    T _end = 0;
    T step = 1;

public:
    class iterator;

/** @brief The default constructor for the `ash::range` class.
 * 
 * The default values for `start` is `0`, `end` is `0`, and `step` is `1`.
 */
    range() {}

/** @brief This constructor only receives a value `end`.
 * 
 * The default values for `start` is `0` and `step` is `1`.
 * 
 * @param end   The exclusive end of the range.
 * 
 * @exception `std::invalid_argument` if `end` is less than `0` (the default `start`).
 */
    range(T end) : _end(end)
    {
        if (end < 0)
            throw std::invalid_argument("Start cannot reach the _end.");
    }

/** @brief This constructor receives values for `start` and `end`.
 * 
 * The default value for `step` is `1`.
 * 
 * @param start   The inclusive start of the range.
 * @param end   The exclusive end of the range.
 * 
 * @exception `std::invalid_argument` if `end` is less than `start`.
 */
    range(T start, T end) : start(start), _end(end)
    {
        if (start > end)
            throw std::invalid_argument("Start cannot reach the _end.");
    }

/** @brief This constructor receives values for `start`, `end`, and `step`.
 * 
 * @param start   The inclusive start of the range.
 * @param end   The exclusive end of the range.
 * @param step  The value by which the range progresses at each step. Can be positive or negative.
 * 
 * @exception `std::invalid_argument` if `step` is `0`.
 * @exception `std::invalid_argument` if `start` cannot reach `end` considering the sign of `step`.
 */
    range(T start, T end, T step) : start(start), _end(end), step(step)
    {
        if (step == 0)
            throw std::invalid_argument("Step for range cannot be zero.");
        if ((end > start && step < 0) || (end < start && step > 0))
            throw std::invalid_argument("Start cannot reach the _end.");
    }

/** @brief This method returns an `ash::range<T>::iterator` pointing to `start`.
 * 
 * The computational complexity is O(1).
 */
    iterator begin() const
    {
        return iterator(start, step);
    }

/** @brief This method returns an `ash::range<T>::iterator` pointing to the first element that goes out of range.
 * 
 * The computational complexity is O(1).
 * 
 * If `step` is positive, this method returns an `iterator`
 * pointing to the first element not less than `end` that is expected to be iterated.
 * 
 * If `step` is negative, this method returns an `iterator`
 * pointing to the first element not greater than `end` that is expected to be iterated.
 */
    iterator end() const
    {
        size_t steps = (double)(_end - start) / step; // Step cannot be zero, handled in the constructor
        T real_end = start + steps * step;
        if (real_end < _end)
            real_end += step;
        return iterator(real_end, step);
    }
};

/** @brief A class to provide the bidirectional iteration of an `ash::range<T>` object.
 * 
 * @tparam T    The type used in `ash::range<T>`.
 * 
 * The default values for `target` is `0`, and `step` is `1` if not provided.
 * 
 * If the template argument is not provided, the program uses the default type (that is `int`).
 * 
 * This is a bidirectional iterator, thus the following methods are defined:
 * - `operator++()` and `operator++(int)`
 * 
 * - `operator--()` and `operator--(int)`
 * 
 * Equality of two iterators is calculated using `1e-9` tolerance.
 */
template <class T>
class ash::range<T>::iterator
{
private:
    T target = 0;
    T step = 1;

public:
    iterator() {}
    iterator(T target, T step) : target(target), step(step) {}

    iterator operator++()
    {
        target += step;
        return *this;
    }
    iterator operator++(int)
    {
        iterator temp = *this;
        target += step;
        return temp;
    }
    iterator operator--()
    {
        target -= step;
        return *this;
    }
    iterator operator--(int)
    {
        iterator temp = *this;
        target -= step;
        return temp;
    }
    T operator*() const
    {
        return target;
    }
    bool operator==(const iterator &other)
    {
        return equal(target, other.target, (T)1e-9) && equal(step, other.step, (T)1e-9);
    }
    bool operator!=(const iterator &other)
    {
        return !equal(target, other.target, (T)1e-9) || !equal(step, other.step, (T)1e-9);
    }

private:
    static constexpr T default_tolerance = static_cast<T>(1e-9);
    static bool equal(T left, T right, T tolerance = default_tolerance)
    {
        return std::abs(left - right) <= tolerance;
    }
};

#endif