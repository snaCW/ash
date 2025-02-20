#ifndef ASH_IS_LOSSLESS_CONVERTIBLE_H_INCLUDED
#define ASH_IS_LOSSLESS_CONVERTIBLE_H_INCLUDED

#include <type_traits>
#include <limits>

namespace ash
{
    template <typename From, typename To, typename = void>
    struct is_lossless_convertible : std::false_type {};
}

template
    <typename From,
    typename To,
    std::enable_if_t<std::is_arithmetic<From>::value && std::is_arithmetic<To>::value>>
struct ash::is_lossless_convertible
{
    static constexpr bool value = []() {

        if constexpr (std::is_same_v<From, To>)
            return true;
        
        if constexpr (std::is_floating_point_v<From> && std::)
    }
};

#endif