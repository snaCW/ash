#ifndef ASH_CPLUSPLUS_VERSIONS_COMPATIBILITY_MACROS
#define ASH_CPLUSPLUS_VERSIONS_COMPATIBILITY_MACROS

/// @def __cpp98
/// @brief The standard `long int` number for C++98.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp98 199711L

/// @def __cpp03
/// @brief The standard `long int` number for C++03.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp03 199711L

/// @def __cpp11
/// @brief The standard `long int` number for C++11.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp11 201103L

/// @def __cpp14
/// @brief The standard `long int` number for C++14.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp14 201402L

/// @def __cpp17
/// @brief The standard `long int` number for C++17.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp17 201703L

/// @def __cpp20
/// @brief The standard `long int` number for C++20.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp20 202002L

/// @def __cpp23
/// @brief The standard `long int` number for C++23.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp23 202302L

/// @def __cpp26
/// @brief The standard `long int` number for C++26.
/// @note Even though it is more usual to use the number itself, we find
/// this macro more readable.
#define __cpp26 202400L

#if __cplusplus >= __cpp17

/// @def if_constexpr
/// @brief Expands to `if constexpr` in C++17 and later, and to `if` in before C++17.
/// @note C++14 and earlier don't support `if constexpr`. `if_constexpr` is much cleaner than
/// the usual `if _GLIBCXX17_CONSTEXPR`. The whole `CXX17` thing is just boiler-plate
/// in the code because it's obvious C++14 and earlier doesn't have `if constexpr`.
///
/// If you are seeing this, it means you are on C++17 or later.
#define if_constexpr \
    if constexpr

#else // __cplusplus < __cpp17

/// @def if_constexpr
/// @brief Expands to `if constexpr` in C++17 and later, and to `if` in before C++17.
/// @note C++14 and earlier don't support `if constexpr`. `if_constexpr` is much cleaner than
/// the usual `if _GLIBCXX17_CONSTEXPR`. The whole `CXX17` thing is just boiler-plate
/// in the code because it's obvious C++14 and earlier doesn't have `if constexpr`.
///
/// If you are seeing this, it means you are on C++14 or earlier.
#define if_constexpr \
    if

#endif // #if __cplusplus >= __cpp17


/// @def __is_auto_allowed_in_template_parameter__
/// @brief Expands to `true` if you are on C++17 or later.
#define __is_auto_allowed_in_template_parameter__ \
    __cplusplus >= __cpp17


#if __cplusplus >= __cpp14

/// @def __CONSTEXPR_WONT_ASSUME_CONST__
/// @brief In C++14 or later, expands to `constexpr`, otherwise expands to nothing.
/// @note Unfortunately, in C++11 when you define a member function as `constexpr`, the compiler
/// assumes the funtions is `const`. This can results in enexpected behaviors like:
///
/// - `constexpr reference at(size_type)` -> `constexpr reference at(size_type) const`
///
/// - `constexpr const_reference at(size_type) const`
///
/// This ugly behavior is fixed in later versions.
#define __CONSTEXPR14_WONT_ASSUME_CONST__ constexpr

#else // __cplusplus <= __cpp11

/// @def __CONSTEXPR_WONT_ASSUME_CONST__
/// @brief In C++14 or later, expands to `constexpr`, otherwise expands to nothing.
/// @note Unfortunately, in C++11 when you define a member function as `constexpr`, the compiler
/// assumes the funtions is `const`. This can results in enexpected behaviors like:
///
/// - `constexpr reference at(size_type)` -> `constexpr reference at(size_type) const`
///
/// - `constexpr const_reference at(size_type) const`
///
/// This ugly behavior is fixed in later versions.
#define __CONSTEXPR14_WONT_ASSUME_CONST__

#endif // __cplusplus >= __cpp14


#endif // ASH_CPLUSPLUS_VERSIONS_COMPATIBILITY_MACROS