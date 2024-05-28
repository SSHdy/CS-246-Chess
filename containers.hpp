#pragma once
#define CS246_CONTAINERS_HPP

#include <utility>
#include <compare>
#include <type_traits>

#include "code_utils.inc"
/*
NAMESPACE_DDDELTA_START
// explicitly view-only relationship
// used to remove raw pointers

template <typename T>
class ptr_view {
public:
    using ptr_type = T*;
    using element_type = T;
    using ref_type = std::add_lvalue_reference_t<element_type>;

public:
    constexpr ptr_view() : _ptr(nullptr) {}
    constexpr ptr_view(std::nullptr_t) : _ptr(nullptr) {}
    constexpr ptr_view(ptr_type p) : _ptr(p) {}

    constexpr ptr_type get() const { return this->_ptr; }
    constexpr ptr_type reset(ptr_type p) {
        std::swap(p, this->_ptr);
        return p;
    }
    constexpr void swap(ptr_view& other) { std::swap(this->_ptr, other._ptr); }

    constexpr operator bool() const { return static_cast<bool>(this->_ptr); }
    constexpr ptr_type operator ->() const { return this->get(); }
    constexpr ref_type operator *() const { return *this->get(); }
    constexpr ptr_view& operator =(ptr_type p) { this->_ptr = p; return *this; }
    template <typename U>
    constexpr ptr_view& operator =(const ptr_view<U>& p) { this->_ptr = p.get(); return *this; }
    template <typename U>
    constexpr ptr_view& operator =(ptr_view<U>&& p) { this->_ptr = p.get(); return *this; }

private:
    ptr_type _ptr;
};


template <typename T, typename U>
inline constexpr std::strong_ordering operator <=>(ptr_view<T> lhs, ptr_view<U> rhs) {
    return lhs.get() <=> rhs.get();
}
NAMESPACE_DDDELTA_END


namespace std {
template <typename T>
inline constexpr void swap(DDDelta::ptr_view<T>& v1, DDDelta::ptr_view<T>& v2) {
    return v1.swap(v2);
}

template <typename T>
inline constexpr void swap(DDDelta::ptr_view<T>&& v1, DDDelta::ptr_view<T>& v2) {
    return v1.swap(v2);
}


template <typename T>
inline constexpr void swap(DDDelta::ptr_view<T>& v1, DDDelta::ptr_view<T>&& v2) {
    return v1.swap(v2);
}
} // namespace std
*/

NAMESPACE_DDDELTA_START
enum class E_Color : bool;


template <typename T>
struct colored_pair : std::pair<T, T> {
    constexpr colored_pair(T first, T second) : std::pair<T, T>(first, second) {}
    constexpr T& operator [](E_Color color) { return to_underlying(color) ? this->first : this->second; }
    constexpr const T& operator [](E_Color color) const { return to_underlying(color) ? this->first : this->second; }
};
NAMESPACE_DDDELTA_END
