#ifndef ASH_THROW_IF
#define ASH_THROW_IF

#include <stdexcept>

namespace ash {
    template <typename size_type>
    _GLIBCXX14_CONSTEXPR void throw_if_outside_of_capacity(size_type N, size_type n);

    template <typename size_type>
    _GLIBCXX14_CONSTEXPR void throw_if_outside_of_size(size_type size, size_type pos);

    template <typename difference_type>
    _GLIBCXX14_CONSTEXPR void throw_if_difference_is_negetive(difference_type diff);

    template <typename T>
    /// @brief Throws `std::logic_error` if the argument is equal to `nullptr`.
    /// @param pointer The pointer to check
    /// @exception `std::logic_error`
    _GLIBCXX14_CONSTEXPR void throw_if_nullptr(T* pointer);
}

template <typename size_type>
/// @brief Throws `std::out_of_range` if `n` is more than `N` (the capacity).
/// @param N Capacity
/// @param n Size
/// @exception `std::out_of_range`
_GLIBCXX14_CONSTEXPR void ash::throw_if_outside_of_capacity(size_type N, size_type n) {
    if (n > N)
        throw std::out_of_range("Access to the outside of the capacity.");
}

template <typename size_type>
/// @brief Throws `std::out_of_range` if `pos` is more than `size()`.
/// @param size Size
/// @param pos Index to access.
/// @exception `std::out_of_range` 
_GLIBCXX14_CONSTEXPR void ash::throw_if_outside_of_size(size_type size, size_type pos) {
    if (pos > size)
        throw std::out_of_range("Access to the out of the size.");
}

template <typename difference_type>
/// @brief Throws `std::logic_error` if `diff` is less than `0` (negetive difference).
/// @param diff Iterators difference
/// @exception `std::logic_error`
_GLIBCXX14_CONSTEXPR void ash::throw_if_difference_is_negetive(difference_type diff) {
    if (diff < 0)
        throw std::logic_error("The 'first' iterator/pointer cannot reach 'last' (Negetive difference).");
}

template <typename T>
_GLIBCXX14_CONSTEXPR void ash::throw_if_nullptr(T* pointer) {
    if (pointer == nullptr)
        throw std::logic_error("The pointer is null.");
}

#endif