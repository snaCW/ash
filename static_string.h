/*
================================================================================
  ash::basic_static_string - A fixed-size, full constexpr class for strings

  License: MIT
  Author: S. Navid Ashrafi
  GitHub: snaCW

  Description:
    A header-only C++ library providing `ash::basic_static_string<T, N>` and
    `ash::static_string<N>` where `T = char`. The string is stored
    null-terminated in the underlying container.

  Usage:
    #include "static_string.h"

  Macros:
    Upon including this file in your project, the following macro(s) will be
    globally exposed:
      - ASH_STATIC_STRING
      - ASH_copy_and

================================================================================
*/

#ifndef ASH_STATIC_STRING

/// @def ASH_STATIC_STRING
/// @brief Include guard for `static_string.h` file.
#define ASH_STATIC_STRING

#include <array>
#include <string>
#include <string_view>
#include <stdexcept>
#include "algorithm.h"
#include "../ash/type_traits.h"
#include "../ash/cplusplus_versions_compatibility_macros.h"
#include "../ash/throw_if.h"

// These are already included in the above libraries.
// #include <cstddef>
// #include <type_traits>
// #include <utility>
// #include <iostream>


// Basic char utility
namespace ash {
    /// @brief Checks if the input refers to one of the English digits.
    /// @param c Input
    /// @return If the input is in `0...9` or not.
    constexpr bool is_digit(char c) noexcept;

    /// @brief Checks if the input is one of the English letters.
    /// @param c Input
    /// @return If the input is in `a...z || A...Z` or not.
    constexpr bool is_letter(char c) noexcept;

    /// @brief Checks if the input is one of the English lowercase letters.
    /// @param c Input
    /// @return If the input is in `a...z` or not.
    constexpr bool is_lower(char c) noexcept;

    /// @brief Checks if the input is one of the English uppercase letters.
    /// @param c Input
    /// @return If the input is in `A...Z` or not.
    constexpr bool is_upper(char c) noexcept;

    /// @brief Converts a character digit to the equivalent `int`.
    /// @param c Input
    /// @return If the input is in `'0'...'9'` returns `0...9`, otherwise returns `-1`.
    constexpr int to_digit(char c) noexcept;

    /// @brief Converts the input to the equivalent English lowercase letter.
    /// @param c Input
    /// @return If `is_upper(c)` returns `c + 'a' - 'A'`, otherwise returns `c`.
    /// If the input is not an English letter, returns `c`.
    constexpr char to_lower(char c) noexcept;

    /// @brief Converts the input to the equivalent English uppercase letter.
    /// @param c Input
    /// @return If `is_lower(c)` returns `c + 'A' - 'a'`, otherwise returns `c`.
    /// If the input is not an English letter, returns `c`.
    constexpr char to_upper(char c) noexcept;

} // Basic char utility

constexpr bool ash::is_digit(char c) noexcept {
    return (c >= '0' && c <= '9');
}

constexpr bool ash::is_letter(char c) noexcept {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

constexpr bool ash::is_lower(char c) noexcept {
    return (c >= 'a' && c <= 'z');
}

constexpr bool ash::is_upper(char c) noexcept {
    return (c >= 'A' && c <= 'Z');
}

constexpr int ash::to_digit(char c) noexcept {
    return is_digit(c) ? c - '0' : -1;
}

constexpr char ash::to_lower(char c) noexcept {
    return ash::is_upper(c) ? (char)((int)c + 'a' - 'A') : c;
}

constexpr char ash::to_upper(char c) noexcept {
    return ash::is_lower(c) ? (char)((int)c + 'A' - 'a') : c;
}

namespace ash {
    template <typename CharT>
    _GLIBCXX14_CONSTEXPR std::size_t strlen(CharT* str) noexcept {
    constexpr CharT null = static_cast<CharT>('\0');

        std::size_t len = 0;
        while (*str != null) {
            ++len;
            ++str;
        }
        
        return len;
    }
}

// Declaration of `ash::basic_static_string`.

namespace ash {
    /// @struct basic_static_string
    /// @brief A fully constexpr wrapper over `std::array<CharT, N + 1>` which
    /// behaves as a compile time string. This is exactly like `std::basic_string<CharT>` but the capacity
    /// is always known at compile time.
    /// @tparam CharT Character-like type of each element.
    /// @tparam N Capacity.
    /// @note The buffer actually stores the string as null terminated (hence the `N + 1`).
    /// However, This class completely acts as if there is no such thing.
    /// (e.g. `back()` never returns `buffer[N]`.)
    template <class CharT, std::size_t N>
    class basic_static_string;

    /// @struct static_string
    /// @brief A fully constexpr wrapper over `std::array<char, N + 1>` which
    /// behaves as a compile time string. This is exactly like `std::string` but the capacity
    /// is always known at compile time.
    /// @tparam N Capacity.
    /// @note The buffer actually stores the string as null terminated (hence the `N + 1`).
    /// However, This class completely acts as if there is no such thing.
    /// (e.g. `back()` never returns `buffer[N]`.)
    template <std::size_t N>
    using static_string = basic_static_string<char, N>;

    /// @struct static_wstring
    /// @brief A fully constexpr wrapper over `std::array<wchar_t, N + 1>` which
    /// behaves as a compile time string. This is exactly like `std::wstring` but the capacity
    /// is always known at compile time.
    /// @tparam N Capacity.
    /// @note The buffer actually stores the string as null terminated (hence the `N + 1`).
    /// However, This class completely acts as if there is no such thing.
    /// (e.g. `back()` never returns `buffer[N]`.)
    template <std::size_t N>
    using static_wstring = basic_static_string<wchar_t, N>;

#ifdef _GLIBCXX_USE_CHAR8_T
    /// @struct static_u8string
    /// @brief A fully constexpr wrapper over `std::array<char8_t, N + 1>` which
    /// behaves as a compile time string. This is exactly like `std::u8string` but the capacity
    /// is always known at compile time.
    /// @tparam N Capacity.
    /// @note The buffer actually stores the string as null terminated (hence the `N + 1`).
    /// However, This class completely acts as if there is no such thing.
    /// (e.g. `back()` never returns `buffer[N]`.)
    template <std::size_t N>
    using static_u8string = basic_static_string<char8_t, N>;
#endif // _GLIBCXX_USE_CHAR8_T

    /// @struct static_u16string
    /// @brief A fully constexpr wrapper over `std::array<char16_t, N + 1>` which
    /// behaves as a compile time string. This is exactly like `std::u16string` but the capacity
    /// is always known at compile time.
    /// @tparam N Capacity.
    /// @note The buffer actually stores the string as null terminated (hence the `N + 1`).
    /// However, This class completely acts as if there is no such thing.
    /// (e.g. `back()` never returns `buffer[N]`.)
    template <std::size_t N>
    using static_u16string = basic_static_string<char16_t, N>;

    /// @struct static_u32string
    /// @brief A fully constexpr wrapper over `std::array<char32_t, N + 1>` which
    /// behaves as a compile time string. This is exactly like `std::u32string` but the capacity
    /// is always known at compile time.
    /// @tparam N Capacity.
    /// @note The buffer actually stores the string as null terminated (hence the `N + 1`).
    /// However, This class completely acts as if there is no such thing.
    /// (e.g. `back()` never returns `buffer[N]`.)
    template <std::size_t N>
    using static_u32string = basic_static_string<char32_t, N>;

} // Declaration of `ash::basic_static_string`.



// Neccessary type traits for `ash::basic_static_string`.

namespace ash {
    /// @brief False Type (SFINAE): `T` is not the same as `ash::basic_static_string<CharT, N>`.
    /// @tparam T type
    /// @note This is a struct. Use `::value` to access the result. `value` is always `false`.
    template <typename T>
    struct is_basic_static_string : std::false_type {};

    /// @brief True Type (SFINAE): `T` is the same as `ash::basic_static_string<CharT, N>`.
    /// @tparam T type
    /// @note This is a struct. Use `::value` to access the result. `value` is always `true`.
    template <typename CharT, std::size_t N>
    struct is_basic_static_string<basic_static_string<CharT, N>> : std::true_type {};


    template <typename T>
    struct is_array_of_basic_static_strings_impl : std::false_type {};

    template <typename U, std::size_t N>
    struct is_array_of_basic_static_strings_impl<std::array<U, N>>
        : is_basic_static_string<ash::remove_cvref_t<U>> {};

    /// @brief False/True Type (SFINAE): Is `T` the same as `std::array<U, arr_N>`
    /// where `U` is `ash::static_basic_string<CharT, str_N>`?
    /// @tparam T type
    /// This is a struct, Use `::value` to access the result.
    template <typename T>
    struct is_array_of_basic_static_strings
        : is_array_of_basic_static_strings_impl<ash::remove_cvref_t<T>> {};



    /// @brief Enable if the template argument has the same type as an `std::array` of `ash::static_basic_string`s.
    /// @tparam T type
    /// @tparam Eval If enabled, evaluates to this type. Default: `int`
    template <typename T, typename Eval = int>
    using enable_if_is_array_of_basic_static_strings_t = ash::enable_if_t<is_array_of_basic_static_strings<T>::value, Eval>;

    /// @brief Enable if the template argument has the same type as an `ash::static_basic_string`.
    /// @tparam T type
    /// @tparam Eval If enabled, evaluates to this type. Default: `int`
    template <typename T, typename Eval = int>
    using enable_if_is_basic_static_string_t = ash::enable_if_t<is_basic_static_string<T>::value, Eval>;

} // Neccessary type traits for `ash::basic_static_string`.


template <class CharT, std::size_t N>
class ash::basic_static_string {
    template <typename, std::size_t>
    friend class basic_static_string; // Friends all the other `basic_static_string`s with other template params.

// Nested types

protected:
#if __cplusplus >= __cpp17
    /// @brief A Helper type to avoid boilder-plate.
    /// @tparam M Number of elements.
    template <std::size_t M>
    using array_t = std::array<CharT, M>;
#else
    /// @brief A Helper type to avoid boilder-plate.
    /// @tparam M Number of elements.
    template <std::size_t M>
    using array_t = CharT[M];
#endif

public:
    /// @brief Type of the internal buffer.
    using buffer_type = array_t<N + 1>;

#if __cplusplus >= __cpp17

    using value_type = typename buffer_type::value_type;
    using size_type = typename buffer_type::size_type;
    using difference_type = typename buffer_type::difference_type;

    using reference = value_type&;
    using const_reference = const value_type&;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = typename buffer_type::iterator;
    using const_iterator = typename buffer_type::const_iterator;
    using reverse_iterator = typename buffer_type::reverse_iterator;
    using const_reverse_iterator = typename buffer_type::const_reverse_iterator;

#else // __cplusplus < __cpp17

    using value_type = CharT;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = value_type&;
    using const_reference = const value_type&;

    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<reverse_iterator>;

#endif // __cplusplus >= __cpp17

    using char_traits = std::char_traits<CharT>;

    template <size_type M>
    using c_array_t = CharT[M];

    template <size_type M>
    using other_t = basic_static_string<CharT, M>;

protected:
#if __cplusplus >= __cpp17
    /// @brief A Helper type to avoid boilder-plate for `string_view`.
    using sv_type = std::basic_string_view<value_type>;
#endif

// Actual data

// Not initializing the buffer here is preferable. It's because we're going to fill the
// buffer in the constructors anyway, so it's better to not double fill the buffer.
// However, only C++20 and later support this...

// C++14 doesn't have `constexpr` iterators so we should just use a normal array.

#if __cplusplus >= __cpp20
    /// @brief The underlying buffer.
    buffer_type buffer;
#elif __cplusplus >= __cpp17
    /// @brief The underlying buffer.
    buffer_type buffer {};
#else
    /// @brief The underlying buffer.
    buffer_type buffer {};
#endif

    /// @brief Size of the string.
    size_type __size = 0;

public:

// Constructors

    _GLIBCXX14_CONSTEXPR basic_static_string() noexcept = default;

    /// @brief Constructs a string with `count` copies of character `ch`.
    /// @param count Count of copies
    /// @param ch The character
    /// @exception `std::out_of_range` if `count` is more than `N`.
    _GLIBCXX14_CONSTEXPR basic_static_string(size_type count, CharT ch);

    /// @brief Constructs a string with the contents of the range [`first`, `last`).
    /// @param first Starting iterator (including).
    /// @param last Ending iterator (excluding).
    /// @exception `std::logic_error` if `last` is before `first`. It's good to note
    /// using `rbegin()` and `rend()` won't cause trouble if you pass them in the
    /// same order.
    /// @exception `std::out_of_range` if the iterator difference is more than `N`.
    template <class InputIt>
    _GLIBCXX14_CONSTEXPR basic_static_string(InputIt first, InputIt last);

    // TODO
    // template< container-compatible-range<CharT> R >
    // constexpr basic_static_string(std::from_range_t, R&& rg);
    
    /// @brief Constructs a string with the contents of the range [`str`, `str + count`).
    /// If [`str`, `str + count`) is not a valid range, the behavior is undefined.
    /// @param str String pointer
    /// @param count Elements count
    /// @exception `std::logic_error` if the pointer is equal to nullptr.
    /// @exception `std::out_of_range` if `count` is more than `N`.
    _GLIBCXX14_CONSTEXPR basic_static_string(const CharT* str, size_type count);

    /// @brief Equivalent to `basic_static_string(str, std::strlen(str))`
    /// @param str String pointer
    /// @exception `std::logic_error` if the argument is equal to nullptr.
    /// @exception `std::out_of_range` if `strlen(str)` is more than `N`.
    template <
        typename pointer_type,
        typename = ash::enable_if_t<
            std::is_pointer<pointer_type>::value &&
            !std::is_array<pointer_type>::value
        >
    >
    _GLIBCXX14_CONSTEXPR basic_static_string(const pointer_type str);

    /// @brief `ash::basic_static_string` cannot be constructed from nullptr.
    /// @note Deleted function. 
    _GLIBCXX14_CONSTEXPR basic_static_string(std::nullptr_t) = delete;

    /// @brief Tries to assume that `StringViewLike` type can be a `std::string_view` and then
    /// construct a string from it.
    /// @tparam StringViewLike Type of the argument.
    /// @param str Any object that has `::value_type` and `being()` and `end()` iterators.
    /// @exception `static_assert` error if `StringViewLike::value_type` is not implicitly
    /// convertible to `CharT`.
    /// @exception `std::out_of_range` if `str.end() - str.begin()` is more than `N`.
    /// @note This constructor won't participate in overload resolution if `StringViewLike`
    /// is a pointer type.
    template <
        class StringViewLike,
        typename = ash::enable_if_t<
            !std::is_pointer<StringViewLike>::value
        >
    >
    explicit _GLIBCXX14_CONSTEXPR basic_static_string(const StringViewLike& str);

    /// @brief Tries to assume that `StringViewLike` type can be a `std::string_view` and then
    /// construct a string from it.
    /// @tparam StringViewLike Type of the argument.
    /// @param str Any object that has `::value_type` and `being()` and `end()` iterators.
    /// @param pos Starting index
    /// @param count The number of elements to copy.
    /// @exception `static_assert` error if `StringViewLike::value_type` is not implicitly
    /// convertible to `CharT`.
    /// @exception `std::out_of_range` if `count` is more than `N`.
    /// @exception `std::out_of_range` if `pos + count - 1` is out of range according to `len = str.end() - str.begin()`.
    template <class StringViewLike>
    _GLIBCXX14_CONSTEXPR basic_static_string(const StringViewLike& str, size_type pos, size_type count);

    /// @brief Constructs a string from a literal string.
    /// @param str The literal string
    /// @exception `std::out_of_range` if `str` size is more than `N`.
    template <std::size_t array_N>
    _GLIBCXX14_CONSTEXPR basic_static_string(const CharT (&str)[array_N]);

    /// @brief [Copy-] Constructs a string with the contents of other.
    /// @param other Other `basic_static_string` object.
    /// @exception `std::out_of_range` if `other.size()` is more than `N`.
    /// @note `other.capacity()` can be more or less then `N`, it doesn't matter. Only `other.size()`
    /// should fit into `N`.
    template <std::size_t other_N>
    _GLIBCXX14_CONSTEXPR basic_static_string(const other_t<other_N>& other);

    /// @brief [Move-] Constructs a string with the contents of other.
    /// @param other Other `basic_static_string` object.
    /// @exception `std::out_of_range` if `other.size()` is more than `N`.
    /// @note When the move finishes, `other` is in a valid state (`other.size() == 0`).
    /// @note `other.capacity()` can be more or less then `N`, it doesn't matter. Only `other.size()`
    /// should fit into `N`.
    template <std::size_t other_N>
    _GLIBCXX14_CONSTEXPR basic_static_string(other_t<other_N>&& other);

    /// @brief [Copy-] Constructs a string with the contents of other.
    /// @param other Other `basic_static_string` object.
    /// @exception `std::out_of_range` if `other.size()` is more than `N`.
    _GLIBCXX14_CONSTEXPR basic_static_string(const other_t<N>& other);

    /// @brief [Move-] Constructs a string with the contents of other.
    /// @param other Other `basic_static_string` object.
    /// @exception `std::out_of_range` if `other.size()` is more than `N`.
    /// @note When the move finishes, `other` is in a valid state (`other.size() == 0`).
    /// @note The complexity is *linear* in the size of the string.
    _GLIBCXX14_CONSTEXPR basic_static_string(other_t<N>&& other);

    /// @brief [Copy-] Constructs a string with the contents of the range [`other.begin() + pos`, `other.end()`).
    /// @param other Other `basic_static_string` object.
    /// @param pos Starting index.
    /// @exception `std::out_of_range` if `pos` is equal or more than `other.size()`.
    /// @exception `std::out_of_range` if `other.size() - pos` is more than `N`.
    template <std::size_t other_N>
    _GLIBCXX14_CONSTEXPR basic_static_string(const other_t<other_N>& other, size_type pos);

    /// @brief [Move-] Constructs a string with the contents of the range [`other.begin() + pos`, `other.end()`).
    /// @param other Other `basic_static_string` object.
    /// @param pos Starting index.
    /// @exception `std::out_of_range` if `pos` is equal or more than `other.size()`.
    /// @exception `std::out_of_range` if `other.size() - pos` is more than `N`.
    /// @note When the move finishes, `other` is in a valid state (`other.size() == 0`).
    /// @note The complexity is *linear* in the size of the string.
    template <std::size_t other_N>
    _GLIBCXX14_CONSTEXPR basic_static_string(other_t<other_N>&& other, size_type pos);

    /// @brief [Copy-] Constructs a string with the contents of the range 
    /// [`other.begin() + pos`, `other.begin() + pos + count`).
    /// @param other Other `basic_static_string` object.
    /// @param pos Starting index.
    /// @param count The number of elements to copy.
    /// @exception `std::out_of_range` if `pos + count - 1` is equal or more than `other.size()`.
    /// @exception `std::out_of_range` if `count` is more than `N`.
    template <std::size_t other_N>
    _GLIBCXX14_CONSTEXPR basic_static_string(const other_t<other_N>& other, size_type pos, size_type count);

    /// @brief [Move-] Constructs a string with the contents of the range 
    /// [`other.begin() + pos`, `other.begin() + pos + count`).
    /// @param other Other `basic_static_string` object.
    /// @param pos Starting index.
    /// @param count The number of elements to move.
    /// @exception `std::out_of_range` if `pos` is equal or more than `other.size()`.
    /// @exception `std::out_of_range` if `count` is more than `N`.
    /// @note When the move finishes, `other` is in a valid state (`other.size() == 0`).
    /// @note The complexity is *linear* in the size of the string.
    template <std::size_t other_N>
    _GLIBCXX14_CONSTEXPR basic_static_string(other_t<other_N>&& other, size_type pos, size_type count);
};


#define ASH_bss_template \
    template <class CharT, std::size_t N>

#define ASH_bss_name \
    ash::basic_static_string<CharT, N>

ASH_bss_template
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(size_type count, CharT ch) {
    ash::throw_if_outside_of_capacity(N, count);

    __size = count;
    ash::fill_with_value(std::begin(buffer), std::begin(buffer) + count, ch);

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + count, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <class InputIt>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(InputIt first, InputIt last) {
    difference_type len = last - first;
    ash::throw_if_difference_is_negetive(len);
    ash::throw_if_outside_of_capacity(N, (size_type)len);

    __size = len;
    ash::fill_from_iterator(std::begin(buffer), first, len);

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + len, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const CharT* str, size_type count) {
    ash::throw_if_nullptr(str);
    ash::throw_if_outside_of_capacity(N, count);
    ash::fill_from_iterator(std::begin(buffer), str, count);
    __size = count;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + count, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <
    typename pointer_type,
    typename
>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const pointer_type str) {
    ash::throw_if_nullptr(str);

    std::size_t count = ash::strlen(str);
    
    ash::throw_if_outside_of_capacity(N, count);
    ash::fill_from_iterator(std::begin(buffer), str, count);
    __size = count;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + count, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <
    class StringViewLike,
    typename
>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const StringViewLike& str) {
    static_assert(ash::is_string_view_like<StringViewLike>::value, "The type has no `begin()` or `end()` iterators or `::value_type` dependant typename.");

    constexpr bool is_array = std::is_array<StringViewLike>::value;
    using elem_t = typename ash::remove_cvref_t<StringViewLike>::value_type;

    static_assert(std::is_convertible<elem_t, CharT>::value, "Cannot implicitly convert the argument to a view.");

    size_type len = str.end() - str.begin();
    ash::throw_if_outside_of_capacity(N, len);

    ash::fill_from_iterator(std::begin(buffer), str.begin(), len);
    __size = len;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + len, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <class StringViewLike>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const StringViewLike& str, size_type pos, size_type count) {
    static_assert(ash::is_string_view_like<StringViewLike>::value, "The type has no `begin()` or `end()` iterators or `::value_type` dependant typename.");

    using elem_t = typename ash::remove_cvref_t<StringViewLike>::value_type;
    static_assert(std::is_convertible<elem_t, CharT>::value, "Cannot implicitly convert the argument to a view.");

    size_type len = str.end() - str.begin();
    ash::throw_if_outside_of_capacity(N, count);
    ash::throw_if_outside_of_size(len, pos + count - 1);

    ash::fill_from_iterator(std::begin(buffer), str.begin() + pos, count);
    __size = count;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + count, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <std::size_t array_N>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const CharT (&str)[array_N]) {
    size_type len = array_N - 1;
    ash::throw_if_outside_of_capacity(N, len);

    ash::fill_from_iterator(std::begin(buffer), std::begin(str), len);

    __size = len;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <std::size_t other_N>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const other_t<other_N>& other) {
    ash::throw_if_outside_of_capacity(N, other.__size);

    ash::fill_from_iterator(std::begin(buffer), std::begin(other.buffer), other.__size);
    __size = other.__size;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <std::size_t other_N>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(other_t<other_N>&& other) {
    ash::throw_if_outside_of_capacity(N, other.__size);

    auto it = std::begin(buffer);
    for (size_type i = 0; i < other.__size; ++i) {
        ash::forward_value_to_iterator(std::move(other.buffer[i]), it);
        ++it;
    }

    __size = other.__size;
    other.__size = 0;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const other_t<N>& other) {
    ash::throw_if_outside_of_capacity(N, other.__size);

    ash::fill_from_iterator(std::begin(buffer), std::begin(other.buffer), other.__size);
    __size = other.__size;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(other_t<N>&& other) {
    ash::throw_if_outside_of_capacity(N, other.__size);

    auto it = std::begin(buffer);
    for (size_type i = 0; i < other.__size; ++i) {
        ash::forward_value_to_iterator(std::move(other.buffer[i]), it);
        ++it;
    }

    __size = other.__size;
    other.__size = 0;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <std::size_t other_N>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const other_t<other_N>& other, size_type pos) {
    ash::throw_if_outside_of_size(other.__size, pos);

    size_type len = other.__size - pos;
    ash::throw_if_outside_of_capacity(N, len);

    ash::fill_from_iterator(std::begin(buffer), std::begin(other.buffer) + pos, len);
    __size = len;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <std::size_t other_N>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(other_t<other_N>&& other, size_type pos) {
    ash::throw_if_outside_of_size(other.__size, pos);

    size_type len = other.__size - pos;
    ash::throw_if_outside_of_capacity(N, len);

    auto it = std::begin(buffer);
    for (size_type i = 0; i < other.__size; ++i) {
        ash::forward_value_to_iterator(std::move(other.buffer[i + pos]), it);
        ++it;
    }

    __size = len;
    other.__size = 0;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <std::size_t other_N>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(const other_t<other_N>& other, size_type pos, size_type count) {
    ash::throw_if_outside_of_size(other.__size, pos + count - 1);

    ash::throw_if_outside_of_capacity(N, count);

    ash::fill_from_iterator(std::begin(buffer), std::begin(other.buffer) + pos, count);
    __size = count;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}

ASH_bss_template
template <std::size_t other_N>
_GLIBCXX14_CONSTEXPR ASH_bss_name::basic_static_string(other_t<other_N>&& other, size_type pos, size_type count) {
    ash::throw_if_outside_of_size(other.__size, pos + count - 1);

    ash::throw_if_outside_of_capacity(N, count);

    auto it = std::begin(buffer);
    for (size_type i = 0; i < count; ++i) {
        ash::forward_value_to_iterator(std::move(other.buffer[i + pos]), it);
        ++it;
    }

    __size = count;
    other.__size = 0;

    // In C++20 and later we didn't initialize the buffer, so we should fill it here.
#if __cplusplus >= __cpp20
    ash::fill_with_value(buffer.begin() + __size, buffer.end(), __default_value__(CharT));
#endif // >= C++20
}



#endif // ASH_STATIC_STRING