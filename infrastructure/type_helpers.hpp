#ifndef NGE_TYPE_HELPERS_HPP
#define NGE_TYPE_HELPERS_HPP

#include <cstddef>

template<typename T1, typename T2>
inline std::size_t constexpr offset_of(T1 T2::*member) {
    constexpr T2 object { };
    return std::size_t(&(object.*member)) - std::size_t(&object);
}

#endif //NGE_TYPE_HELPERS_HPP
