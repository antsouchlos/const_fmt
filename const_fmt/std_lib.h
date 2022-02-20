#ifndef CONST_FMT_STD_LIB_H
#define CONST_FMT_STD_LIB_H


#ifndef CONST_FMT_NO_CPP_STD_LIB

#include <array>
#include <cstring>

#else

#include <stdint.h>
#include <string.h>


namespace std {


using size_t = uint16_t;


/*
 *
 * type_traits
 *
 */


// clang-format off

template<typename T> struct remove_reference      { using type = T; };
template<typename T> struct remove_reference<T&>  { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };

template<typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

// clang-format on


/*
 *
 * utility
 *
 */


template <typename T>
std::remove_reference_t<T>&& move(T&& arg) noexcept {
    return reinterpret_cast<std::remove_reference_t<T>&&>(arg);
}

template <typename T>
void swap(T& t1, T& t2) {
    T temp = std::move(t1);
    t1     = std::move(t2);
    t2     = std::move(temp);
}


/*
 *
 * std::array
 *
 */


// TODO: Is std::size_t really the best bet here?
template <typename data_t, std::size_t t_size>
class array {
public:
    template<typename... args_t>
    constexpr array(args_t... args) noexcept : m_data{args...} {
        static_assert(sizeof...(args) == t_size, "Invalid number of arguments");
    }

    constexpr array() noexcept = default;
    constexpr array(array&) = default;
    constexpr array(array&&) = default;

    constexpr array& operator=(array& other) = default;
    constexpr array& operator=(array&& other) = default;

    constexpr void swap(array<data_t, t_size>& other) noexcept {
        for (int i = 0; i < t_size; ++i) {
            using std::swap;
            swap(m_data[i], other.m_data[i]);
        }
    }


    constexpr data_t& operator[](std::size_t index) noexcept {
        return m_data[index];
    }
    constexpr const data_t& operator[](std::size_t index) const noexcept {
        return m_data[index];
    }


    using iterator       = data_t*;
    using const_iterator = const data_t*;

    constexpr iterator begin() noexcept {
        return &(m_data[0]);
    }
    constexpr iterator end() noexcept {
        return (&(m_data[t_size - 1]) + 1);
    }

    constexpr const_iterator cbegin() const noexcept {
        return &(m_data[0]);
    }
    constexpr const_iterator cend() const noexcept {
        return (&(m_data[t_size - 1]) + 1);
    }

    data_t m_data[t_size];
};


} // namespace std


#endif


#endif // CONST_FMT_STD_LIB_H
