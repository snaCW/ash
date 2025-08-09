/*
================================================================================
  ash/type_traits.h - A C++11 compatible type-traits library which provides a
  unified API to many standard type-traits for C++11 and later.

  License: MIT
  Author: S. Navid Ashrafi
  GitHub: snaCW

  Description:
    A fully compatible C++11 header-only C++ library to make it easier to have
    the same code across multiple versions of C++.

    For example, you can use `ash::enable_if_t` in your project, and no matter
    which C++ version you compile with (C++11 or later), the compiler uses
    the proper `std::enable_if_t` or `std::__enable_if_t`.

  Usage:
    #include "ash/type_traits.h"

    To enforce include clarity, this library includes `"../ash/type_traits.h"`.
    This forces the user to place this file into an `ash` folder. This is
    neccessary because the name of this file is the same as the `std` version,
    and we don't want to see something like this:
    ```
    #include <type_traits.h>
    #include "type_traits.h"
    ```

  Macros:
    Upon including this file in your project, the following macro(s) will be
    globally exposed:
      - ASH_TYPE_TRAITS

================================================================================
*/

#ifndef ASH_TYPE_TRAITS
#define ASH_TYPE_TRAITS

#include <type_traits>
#include "../ash/type_traits.h"
#include "../ash/cplusplus_versions_compatibility_macros.h"

// Define traits that are not in C++11

namespace ash {

// Define `bool_constant`

#if __cplusplus < __cpp17
    template <bool value>
    using bool_constant = std::integral_constant<bool, value>;
#else
    template <bool value>
    using bool_constant = std::bool_constant<value>;
#endif


// Define `has_unique_object_representations`

#if __cplusplus < __cpp17
    template <typename T>
    struct has_unique_object_representations
        : ash::bool_constant<
            __has_unique_object_representations(typename std::remove_cv<typename std::remove_all_extents<T>::type>::type)
        > {
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<T>{}),
        "template argument must be a complete class or an unbounded array");
    };
#else
    template <typename T>
    using has_unique_object_representations = std::has_unique_object_representations<T>;
#endif

// Define `disjunction`

#if __cplusplus < __cpp17
    template <typename /* = void */, typename _B1, typename... _Bn>
    struct __disjunction_impl {
        using type = _B1;
    };

    template <typename _B1, typename _B2, typename... _Bn>
    struct __disjunction_impl<std::__enable_if_t<!bool(_B1::value)>, _B1, _B2, _Bn...> {
        using type = typename __disjunction_impl<void, _B2, _Bn...>::type;
    };

    template <typename /* = void */, typename _B1, typename... _Bn>
    struct __conjunction_impl {
        using type = _B1;
    };

    template <typename _B1, typename _B2, typename... _Bn>
    struct __conjunction_impl<std::__enable_if_t<bool(_B1::value)>, _B1, _B2, _Bn...> {
        using type = typename __conjunction_impl<void, _B2, _Bn...>::type;
    };

    template <typename... _Bn>
    struct conjunction : __conjunction_impl<void, _Bn...>::type {};

    template <>
    struct conjunction<> : std::true_type {};

    template <typename... _Bn>
    struct disjunction : __disjunction_impl<void, _Bn...>::type {};

    template<>
    struct disjunction<> : std::false_type {};

    template<typename _Pp>
    struct negation : std::__not_<_Pp>::type {};
#else
    template <typename... B>
    using disjunction = std::disjunction<B...>;

    template <typename... B>
    using conjunction = std::conjunction<B...>;

    template <typename B>
    using negation = std::negation<B>;
#endif

// Define `is_aggregate`

#if __cplusplus >= __cpp17
    template <typename T>
    using is_aggregate = std::is_aggregate<T>;
#else
    template <typename T>
    struct is_aggregate : ash::bool_constant<__is_aggregate(typename std::remove_cv<T>::type)> {};
#endif

// Define `is_implicit_lifetime`

    template <typename T, typename = void>
    struct is_implicit_lifetime_class_type : std::false_type {};

    template <typename T>
    struct is_implicit_lifetime_class_type<T,
        typename std::enable_if<
            !std::is_array<T>::value &&
            ash::is_aggregate<T>::value &&
            std::is_trivially_default_constructible<T>::value &&
            std::is_trivially_destructible<T>::value &&
            std::is_trivially_copyable<T>::value
        >::type
    > : std::true_type {};

#ifdef __cpp_lib_is_implicit_lifetime
    template <typename T>
    using is_implicit_lifetime = std::is_implicit_lifetime<T>;

#else

    template <typename T, typename = void>
    struct is_implicit_lifetime : std::false_type {};

    template <typename T>
    struct is_implicit_lifetime<T, typename std::enable_if<std::is_scalar<T>::value>::type> : std::true_type {};

    template <typename T>
    struct is_implicit_lifetime<T, typename std::enable_if<ash::is_implicit_lifetime_class_type<T>::value>::type> : std::true_type {};

    template <typename T>
    struct is_implicit_lifetime<T, typename std::enable_if<std::is_array<T>::value>::type> : std::true_type {};

#endif

// Define `is_final`
#if __cplusplus >= __cpp14
    template <typename T>
    using is_final = std::is_final<T>;
#else
    template <typename T>
    struct is_final : public std::integral_constant<bool, __is_final(T)> {};
#endif

// Define `is_bounded_array` and `is_unbounded_array`
#if __cplusplus >= __cpp20
    template <typename T>
    using is_bounded_array = std::is_bounded_array<T>;

    template <typename T>
    using is_unbounded_array = std::is_unbounded_array<T>;
#else
    template <typename T>
    using is_bounded_array = std::__is_array_known_bounds<T>;

    template <typename T>
    using is_unbounded_array = std::__is_array_unknown_bounds<T>;
#endif

// Define `is_scoped_enum`
#if __cplusplus >= __cpp23
    template <typename T>
    using is_scoped_enum = std::is_scoped_enum<T>;
#else
    template<typename T, typename = void>
    struct is_scoped_enum : std::false_type {};

    template<typename T>
    struct is_scoped_enum<T,
        typename std::enable_if<
            std::is_enum<T>::value
        >::type
    > : ash::bool_constant<!std::is_convertible<T, int>::value> {};
#endif

// Define `is_swappable`

    namespace __swappable_with_details {
#if __cplusplus >= __cpp17
        using __do_is_swappable_with_impl = std::__swappable_with_details::__do_is_swappable_with_impl;
        using __do_is_nothrow_swappable_with_impl = std::__swappable_with_details::__do_is_nothrow_swappable_with_impl;
#else
        struct __do_is_swappable_with_impl {

            template <typename T, typename U,
                typename = decltype(std::swap(std::declval<T>(), std::declval<U>())),
                typename = decltype(std::swap(std::declval<U>(), std::declval<T>()))
            >
            static std::true_type __test(int);

            template <typename, typename>
            static std::false_type __test(...);
        };

        struct __do_is_nothrow_swappable_with_impl {

            template <typename T, typename U>
            static ash::bool_constant<
                noexcept(std::swap(std::declval<T>(), std::declval<U>())) &&
                noexcept(std::swap(std::declval<U>(), std::declval<T>()))
            >
            __test(int);

            template <typename, typename>
            static std::false_type __test(...);
        };
#endif
    } // namespace __swappable_with_details

#if __cplusplus >= __cpp17
    template <typename... Args>
    using __is_swappable_with_impl = std::__is_swappable_with_impl<Args...>;

    template <typename... Args>
    using __is_nothrow_swappable_with_impl = std::__is_nothrow_swappable_with_impl<Args...>;

#else
    template <typename T, typename U>
    struct __is_swappable_with_impl : public ash::__swappable_with_details::__do_is_swappable_with_impl {
        typedef decltype(__test<T, U>(0)) type;
    };

    // Optimization for the homogenous lvalue case, not required:
    template <typename T>
    struct __is_swappable_with_impl<T&, T&> : public std::__swappable_details::__do_is_swappable_impl {
        typedef decltype(__test<T&>(0)) type;
    };

    template <typename T, typename U>
    struct __is_nothrow_swappable_with_impl : public ash::__swappable_with_details::__do_is_nothrow_swappable_with_impl {
        typedef decltype(__test<T, U>(0)) type;
    };

    // Optimization for the homogenous lvalue case, not required:
    template <typename T>
    struct __is_nothrow_swappable_with_impl<T&, T&> : public std::__swappable_details::__do_is_nothrow_swappable_impl {
        typedef decltype(__test<T&>(0)) type;
    };

#endif

#if __cplusplus >= __cpp17
    template <typename T, typename U>
    using is_swappable_with = std::is_swappable_with<T, U>;

    template <typename T>
    using is_swappable = std::is_swappable<T>;

    template <typename T, typename U>
    using is_nothrow_swappable_with = std::is_nothrow_swappable_with<T, U>;

    template <typename T>
    using is_nothrow_swappable = std::is_nothrow_swappable<T>;

#else

    /// is_swappable_with
    template <typename T, typename U>
    struct is_swappable_with : public ash::__is_swappable_with_impl<T, U>::type {
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<T>{}),
        "first template argument must be a complete class or an unbounded array");
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<U>{}),
        "second template argument must be a complete class or an unbounded array");
    };

    /// is_nothrow_swappable_with
    template <typename T, typename U>
    struct is_nothrow_swappable_with : public ash::__is_nothrow_swappable_with_impl<T, U>::type {
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<T>{}),
        "first template argument must be a complete class or an unbounded array");
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<U>{}),
        "second template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    struct is_swappable : public std::__is_swappable_impl<T>::type {
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<T>{}),
        "template argument must be a complete class or an unbounded array");
    };

    /// is_nothrow_swappable
    template<typename T>
    struct is_nothrow_swappable : public std::__is_nothrow_swappable_impl<T>::type {
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<T>{}),
	    "template argument must be a complete class or an unbounded array");
    };

#endif

// Define `reference_constructs_from_temporary` and `reference_converts_from_temporary`
// Unfortunately, Clang doesn't support this C++23 feature on all compilers that support
// C++23.

#if __cplusplus >= __cpp23 && defined(__cpp_lib_reference_from_temporary)
    template <typename T, typename U>
    using reference_constructs_from_temporary = std::reference_constructs_from_temporary<T, U>;

    template <typename T, typename U>
    using reference_converts_from_temporary = std::reference_converts_from_temporary<T, U>;

#elif __has_builtin(__reference_converts_from_temporary) && __has_builtin(__reference_converts_from_temporary)

    template <typename T, typename U>
    struct reference_constructs_from_temporary
    : public ash::bool_constant<__reference_constructs_from_temporary(T, U)> {
        static_assert(
            std::__is_complete_or_unbounded(std::__type_identity<T>{}) &&
            std::__is_complete_or_unbounded(std::__type_identity<U>{}),

	        "template argument must be a complete class or an unbounded array"
        );
    };

    template <typename T, typename U>
    struct reference_converts_from_temporary
    : public ash::bool_constant<__reference_converts_from_temporary(T, U)> {
        static_assert(
            std::__is_complete_or_unbounded(std::__type_identity<T>{}) &&
		    std::__is_complete_or_unbounded(std::__type_identity<U>{}),

	        "template argument must be a complete class or an unbounded array"
        );
    };
#endif

// Define `is_pod`
    template<typename T>
    struct
    _GLIBCXX20_DEPRECATED_SUGGEST("is_standard_layout && is_trivial")
    is_pod : public std::integral_constant<bool, __is_pod(T)> {
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<T>{}),
	    "template argument must be a complete class or an unbounded array");
    };

// Define `is_integral_type`
    template <typename T>
    struct
    _GLIBCXX17_DEPRECATED
    is_literal_type : public std::integral_constant<bool, __is_literal_type(T)> {
        static_assert(std::__is_complete_or_unbounded(std::__type_identity<T>{}),
	    "template argument must be a complete class or an unbounded array");
    };
}

// Value helpers.

#if __cplusplus >= __cpp14
namespace ash {
#define __make_helper_from_std(Name) \
    template <typename T> \
    constexpr bool Name##_v = std::Name<T>::value;

#define __make_helper_from_ash(Name) \
    template <typename T> \
    constexpr bool Name##_v = ash::Name<T>::value;

    __make_helper_from_std(is_void)
    __make_helper_from_std(is_null_pointer)
    __make_helper_from_std(is_integral)
    __make_helper_from_std(is_floating_point)
    __make_helper_from_std(is_array)
    __make_helper_from_std(is_enum)
    __make_helper_from_std(is_union)
    __make_helper_from_std(is_class)
    __make_helper_from_std(is_function)
    __make_helper_from_std(is_pointer)
    __make_helper_from_std(is_lvalue_reference)
    __make_helper_from_std(is_rvalue_reference)
    __make_helper_from_std(is_member_object_pointer)
    __make_helper_from_std(is_member_function_pointer)

    __make_helper_from_std(is_fundamental)
    __make_helper_from_std(is_arithmetic)
    __make_helper_from_std(is_scalar)
    __make_helper_from_std(is_object)
    __make_helper_from_std(is_compound)
    __make_helper_from_std(is_reference)
    __make_helper_from_std(is_member_pointer)

    __make_helper_from_std(is_const)
    __make_helper_from_std(is_volatile)
    __make_helper_from_std(is_trivial)
    __make_helper_from_std(is_trivially_copyable)
    __make_helper_from_std(is_standard_layout)
    __make_helper_from_std(is_empty)
    __make_helper_from_std(is_polymorphic)
    __make_helper_from_std(is_abstract)
    __make_helper_from_std(is_signed)
    __make_helper_from_std(is_unsigned)

    __make_helper_from_std(is_default_constructible)
    __make_helper_from_std(is_trivially_default_constructible)
    __make_helper_from_std(is_nothrow_default_constructible)

    __make_helper_from_std(is_copy_constructible)
    __make_helper_from_std(is_trivially_copy_constructible)
    __make_helper_from_std(is_nothrow_copy_constructible)

    __make_helper_from_std(is_move_constructible)
    __make_helper_from_std(is_trivially_move_constructible)
    __make_helper_from_std(is_nothrow_move_constructible)

    __make_helper_from_std(is_copy_assignable)
    __make_helper_from_std(is_trivially_copy_assignable)
    __make_helper_from_std(is_nothrow_copy_assignable)

    __make_helper_from_std(is_move_assignable)
    __make_helper_from_std(is_trivially_move_assignable)
    __make_helper_from_std(is_nothrow_move_assignable)

    __make_helper_from_std(is_destructible)
    __make_helper_from_std(is_trivially_destructible)
    __make_helper_from_std(is_nothrow_destructible)

    __make_helper_from_std(has_virtual_destructor)

    // __make_helper_from_std(is_void)
    // __make_helper_from_std(is_void)
    // __make_helper_from_std(is_void)

    template <typename T>
    _GLIBCXX20_DEPRECATED_SUGGEST("is_standard_layout_v && is_trivial_v")
    constexpr bool is_pod_v = __is_pod(T);

    template <typename T>
    _GLIBCXX17_DEPRECATED
    constexpr bool is_literal_type_v = __is_literal_type(T);

    template <typename T, class... Args>
    constexpr bool is_constructible_v = std::is_constructible<T, Args...>::value;

    template <typename T, class... Args>
    constexpr bool is_trivially_constructible_v = std::is_trivially_constructible<T, Args...>::value;

    template <typename T, class... Args>
    constexpr bool is_nothrow_constructible_v = std::is_nothrow_constructible<T, Args...>::value;

    template <typename T, typename U>
    constexpr bool is_assignable_v = std::is_assignable<T, U>::value;

    template <typename T, typename U>
    constexpr bool is_trivially_assignable_v = std::is_trivially_assignable<T, U>::value;

    template <typename T, typename U>
    constexpr bool is_nothrow_assignable_v = std::is_nothrow_assignable<T, U>::value;

    template <typename T, typename U>
    constexpr bool is_same_v = std::is_same<T, U>::value;


    __make_helper_from_ash(has_unique_object_representations)
    __make_helper_from_ash(is_final)
    __make_helper_from_ash(is_aggregate)
    __make_helper_from_ash(is_implicit_lifetime)
    __make_helper_from_ash(is_bounded_array)
    __make_helper_from_ash(is_unbounded_array)
    __make_helper_from_ash(is_scoped_enum)
    // __make_helper_from_ash(is_void)
    // __make_helper_from_ash(is_void)

    template <typename T, typename U>
    constexpr bool is_swappable_with_v = ash::is_swappable_with<T, U>::value;

    template <typename T>
    constexpr bool is_swappable_v = ash::is_swappable<T>::value;

    template <typename T, typename U>
    constexpr bool is_nothrow_swappable_with_v = ash::is_nothrow_swappable_with<T, U>::value;

    template <typename T>
    constexpr bool is_nothrow_swappable_v = ash::is_nothrow_swappable<T>::value;

#undef __make_helper_from_std
#undef __make_helper_from_ash
}
#endif // #if __cplusplus >= __cpp14


namespace ash {
    /// @brief Adds support for `remove_reference_t` in C++11.
    /// @tparam T type
    template <typename T>
#if __cplusplus >= __cpp14
        using remove_reference_t = std::remove_reference_t<T>;
#else
        using remove_reference_t = typename std::remove_reference<T>::type;
#endif

    /// @brief Adds support for `conditional_t` in C++11.
    /// @tparam Cond Boolean condition
    /// @tparam Iftrue The type to return if `Cond` is `true`
    /// @tparam Iffalse The type to return if `Cond` is `false`
    template <bool Cond, typename Iftrue, typename Iffalse>
#if __cplusplus >= __cpp14
    using conditional_t = std::conditional_t<Cond, Iftrue, Iffalse>;
#else
    using conditional_t = typename std::conditional<Cond, Iftrue, Iffalse>::type;
#endif
    
    /// @brief Supports both `std::remove_cvref_t` and `std::__remove_cvref_t`
    /// under a unified API.
    /// @tparam T type
    template <typename T>
#if __cplusplus >= __cpp20
        using remove_cvref_t = std::remove_cvref_t<T>;
#else
        using remove_cvref_t = std::__remove_cvref_t<T>;
#endif

    /// @brief Supports both `std::enable_if_t` and `std::__enable_if_t`
    /// under a unified API.
    /// @tparam T type
    template <bool Cond, typename T = void>
#if __cplusplus >= __cpp14
        using enable_if_t = std::enable_if_t<Cond, T>;
#else
        using enable_if_t = std::__enable_if_t<Cond, T>;
#endif

// Define `is_char_type`

    template <typename T>
    struct is_char_type : std::false_type {};

    template <>
    struct is_char_type<char> : std::true_type {};

#ifdef _GLIBCXX_USE_CHAR8_T
    template <>
    struct is_char_type<char8_t> : std::true_type {};
#endif

    template <>
    struct is_char_type<char16_t> : std::true_type {};

    template <>
    struct is_char_type<char32_t> : std::true_type {};
}

#endif