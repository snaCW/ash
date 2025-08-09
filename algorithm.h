#ifndef ASH_ALGORITHM_H
#define ASH_ALGORITHM_H

#include <type_traits>
#include <utility>
#include "../ash/cplusplus_versions_compatibility_macros.h"
#include "../ash/type_traits.h"

#ifndef ASH_copy_and

/// @def ASH_copy_and
/// @brief Provides a `copy` object which you can apply any `Expression`s on.
/// @param Expression Any possible expression(s). All of these expressions can
/// access the `copy` object.
/// @return A lambda function which returns `copy`.
/// @note This macro expands to a lambda function which takes `const auto& obj`
/// as argument. The `copy` object is copied from `obj` and then `Expression` is
/// applied on it. Technically, you can access to both `obj` and `copy` objects
/// in `Expression`.
#define ASH_copy_and(Expression) \
    [](const auto& obj) { \
        auto copy = obj; \
        Expression \
        return copy; \
    }

#endif // #ifndef ASH_copy_and

namespace ash {
    template <typename InputIt, typename T>
    _GLIBCXX14_CONSTEXPR InputIt find(InputIt first, InputIt last, const T& value);

    template <typename InputIt, typename T>
    constexpr bool contains(InputIt first, InputIt last, const T& value);

    template <typename T, typename InputIt>
    _GLIBCXX14_CONSTEXPR void copy_value_to_iterator(const T& value, InputIt it);

    template <typename T, typename InputIt>
    _GLIBCXX14_CONSTEXPR void forward_value_to_iterator(T&& value, InputIt it);

    template <typename InputIt, typename T>
    _GLIBCXX14_CONSTEXPR void fill_with_value(InputIt first, InputIt last, const T& value);

    template <typename InputIt, typename InputIt2, typename size_type>
    _GLIBCXX14_CONSTEXPR void fill_from_iterator(InputIt first, InputIt2 first2, size_type count);

    template <typename T>
    _GLIBCXX14_CONSTEXPR void shift_elements(T& obj, long move_stride, std::size_t index, std::size_t count);

#if __cplusplus >= __cpp14
    /// @brief The default value for `T`.
    /// @tparam T type.
    template <typename T>
    static constexpr T default_value = T();

    /// @brief The 'No Position' value for indexing. `npos` is always equal to the cast
    /// of `-1` to `T`.
    /// @tparam T Integral type, with default type `std::size_t`.
    /// @tparam dummy SFINAE variable to ensure that `T` is an intergral type.
    /// @note This was originally a static member of `basic_static_string<CharT, N>`,
    /// but since the compiler would have create multiple `npos` objects for each
    /// different `CharT` and `N`, we had to define it outside of the class.
    template <typename T = std::size_t, ash::enable_if_t<ash::is_integral_v<T>, int>* dummy = nullptr>
    static constexpr T npos = static_cast<T>(-1);
#endif

#if __cplusplus >= __cpp14
#define __default_value__(T) default_value<T>
#define __npos__(T) npos<T>
#else
#define __default_value__(T) T()
#define __npos__(T) static_cast<T>(-1)
#endif
};

template <typename InputIt, typename T>
_GLIBCXX14_CONSTEXPR InputIt ash::find(InputIt first, InputIt last, const T& value) {
    for (auto it = first; it != last; ++it)
        if (*it == value)
            return it;
        
    return last;
}

template <typename InputIt, typename T>
constexpr bool ash::contains(InputIt first, InputIt last, const T& value) {
    return ash::find(first, last, value) != last;
}

template <typename T, typename InputIt>
_GLIBCXX14_CONSTEXPR void copy_value_to_iterator(const T& value, InputIt it) {
    using elem_t = ash::remove_cvref_t<decltype(*it)>;

    // Since this is going to get a bit complicated, we will return early
    // whenever we have success.

    if_constexpr (std::is_copy_assignable<elem_t>::value) {
        *it = value;
        return;
    }

    if (!__builtin_is_constant_evaluated()) {
        if_constexpr (std::is_copy_constructible<elem_t>::value) {
            (*it).~elem_t();
            new (std::addressof(*it)) elem_t(value);
            return;
        }
    }

    throw std::logic_error("Cannot copy.");
}

template <typename T, typename InputIt>
_GLIBCXX14_CONSTEXPR void forward_value_to_iterator(T&& value, InputIt it) {
    using elem_t = ash::remove_cvref_t<decltype(*it)>;

    // Since this is going to get a bit complicated, we will return early
    // whenever we have success.

    if_constexpr (std::is_move_assignable<elem_t>::value) {
        *it = std::move(value);
        return;
    }

    if (!__builtin_is_constant_evaluated()) {
        if_constexpr (std::is_move_constructible<elem_t>::value) {
            (*it).~elem_t();
            new (std::addressof(*it)) elem_t(std::move(value));
            return;
        }
    }

    if_constexpr (std::is_copy_assignable<elem_t>::value) {
        *it = std::forward(value); // `elem_t` is not move assignable, so forward will copy safely
        return;
    }

    if (!__builtin_is_constant_evaluated()) {
        if_constexpr (std::is_copy_constructible<elem_t>::value) {
            (*it).~elem_t();
            new (std::addressof(*it)) elem_t(std::forward(value));
            return;
        }
    }

    throw std::logic_error("Cannot forward.");
}

template <typename InputIt, typename T>
_GLIBCXX14_CONSTEXPR void ash::fill_with_value(InputIt first, InputIt last, const T& value) {
    using elem_t = ash::remove_cvref_t<decltype(*first)>;

    if (__builtin_is_constant_evaluated()) {
        #if __cplusplus >= 202002L
            std::fill(first, last, value);
        #else
            for (auto it = first; it != last; ++it) {
                copy_value_to_iterator(value, it);
            }
        #endif
    }
    else {
        std::fill(first, last, value);
    }
}

template <typename InputIt, typename InputIt2, typename size_type>
_GLIBCXX14_CONSTEXPR void ash::fill_from_iterator(InputIt first, InputIt2 first2, size_type count) {
    using elem_t = ash::remove_cvref_t<decltype(*first)>;
    using elem2_t = ash::remove_cvref_t<decltype(*first2)>;

    InputIt last = first + count;

    auto it2 = first2;
    for (auto it = first; it != last; ++it) {
        copy_value_to_iterator(*it2, it);

        ++it2;
    }
}

template <typename T>
_GLIBCXX14_CONSTEXPR void ash::shift_elements(T& obj, long move_stride, std::size_t index, std::size_t count) {
    if (move_stride == 0)
        return;

    using elem_t = ash::remove_cvref_t<decltype(obj[0])>;

    static_assert(std::is_same<elem_t, typename T::value_type>::value, "operator[] must return a reference to the value_type");

    bool is_a_forward_move = (move_stride > 0);
    long abs_stride = (move_stride > 0) ? move_stride : -move_stride;
    auto begin = std::begin(obj);

    if (is_a_forward_move) {
        std::size_t last = index + count - 1;
        while (last >= index) {
            forward_value_to_iterator(std::move(obj[last]), begin + last + abs_stride);

            if (last == 0)
                break;
            
            --last;
        }
    }
    else {
        std::size_t last = index + count - 1;
        while (index <= last) {
            forward_value_to_iterator(std::move(obj[last]), begin + index - abs_stride);
            
            ++index;
        }
    }
}

#endif