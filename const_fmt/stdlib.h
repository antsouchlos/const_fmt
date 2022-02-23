#ifndef CONST_FMT_STDLIB_H
#define CONST_FMT_STDLIB_H


/*
 *
 * Disclaimer: Very bad (partially at least) implementation of some features of
 * the C++ Standard Library. Not meant as a full-on stdlib implementation, only
 * for usage in this project (Underlined by the fact, that in this case the
 * namespace std is actually const_fmt::std)
 *
 */


#ifndef CONST_FMT_NO_CPP_STDLIB

#include <array>
#include <cstring>

#else

#include <stdint.h>
#include <string.h>


namespace const_fmt { namespace std {


using size_t = uint16_t;


/*
 *
 * type_traits
 *
 */


struct true_type {
    constexpr static bool value = true;
};

struct false_type {
    constexpr static bool value = false;
};


// clang-format off

template <typename type_t> struct remove_cv                        { typedef type_t type; };
template <typename type_t> struct remove_cv<const type_t>          { typedef type_t type; };
template <typename type_t> struct remove_cv<volatile type_t>       { typedef type_t type; };
template <typename type_t> struct remove_cv<const volatile type_t> { typedef type_t type; };

template <typename type_t> struct remove_const                     { typedef type_t type; };
template <typename type_t> struct remove_const<const type_t>       { typedef type_t type; };

template <typename type_t> struct remove_volatile                  { typedef type_t type; };
template <typename type_t> struct remove_volatile<volatile type_t> { typedef type_t type; };

template <typename type_t> struct remove_reference                 { using type = type_t; };
template <typename type_t> struct remove_reference<type_t &>       { using type = type_t; };
template <typename type_t> struct remove_reference<type_t &&>      { using type = type_t; };

template <typename type_t> using remove_reference_t = typename std::remove_reference<type_t>::type;
template <typename type_t> using remove_cv_t        = typename std::remove_cv<type_t>::type;
template <typename type_t> using remove_const_t     = typename std::remove_const<type_t>::type;
template <typename type_t> using remove_volatile_t  = typename std::remove_volatile<type_t>::type;


template <typename> struct is_integral_helper : public false_type {};

template <> struct is_integral_helper<bool>               : public true_type {};
template <> struct is_integral_helper<char>               : public true_type {};
template <> struct is_integral_helper<signed char>        : public true_type {};
template <> struct is_integral_helper<unsigned char>      : public true_type {};
template <> struct is_integral_helper<wchar_t>            : public true_type {};
template <> struct is_integral_helper<char16_t>           : public true_type {};
template <> struct is_integral_helper<char32_t>           : public true_type {};
template <> struct is_integral_helper<short>              : public true_type {};
template <> struct is_integral_helper<unsigned short>     : public true_type {};
template <> struct is_integral_helper<int>                : public true_type {};
template <> struct is_integral_helper<unsigned int>       : public true_type {};
template <> struct is_integral_helper<long>               : public true_type {};
template <> struct is_integral_helper<unsigned long>      : public true_type {};
template <> struct is_integral_helper<long long>          : public true_type {};
template <> struct is_integral_helper<unsigned long long> : public true_type {};

template <typename type_t>
struct is_integral
        : public is_integral_helper<remove_cv_t<type_t>> {};


template <typename> struct is_floating_point_helper : public false_type {};

template<> struct is_floating_point_helper<float>       : public true_type {};
template<> struct is_floating_point_helper<double>      : public true_type {};
template<> struct is_floating_point_helper<long double> : public true_type {};

template<typename _Tp>
struct is_floating_point
        : public is_floating_point_helper<remove_cv_t<_Tp>>::type {};



constexpr inline bool is_constant_evaluated() noexcept {
    return __builtin_is_constant_evaluated();
}


template <typename type_t> struct make_unsigned    { using type = type_t; };

template <> struct make_unsigned<signed char>      { using type = char; };
template <> struct make_unsigned<unsigned short>   { using type = unsigned short; };
template <> struct make_unsigned<signed int>       { using type = unsigned int; };
template <> struct make_unsigned<signed long>      { using type = unsigned long; };
template <> struct make_unsigned<signed long long> { using type = unsigned long long; };

// clang-format on


/*
 *
 * concepts
 *
 */


// template <typename _Tp>
// concept integral = is_integral_v<_Tp>;
//
// template <typename _Tp>
// concept signed_integral = integral<_Tp> && is_signed_v<_Tp>;
//
// template <typename _Tp>
// concept unsigned_integral = integral<_Tp> && !signed_integral<_Tp>;
//
// template <typename _Tp>
// concept floating_point = is_floating_point_v<_Tp>;


/*
 *
 * algorithm
 *
 */


template <typename input_t, typename output_t>
void copy(input_t* start, input_t* end, output_t* dest_start) {
    memcpy(start, dest_start, end - start);
}


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

template <typename first_t, typename second_t>
struct pair {
    first_t  first;
    second_t second;
};


/*
 *
 * array
 *
 */


// TODO: Is std::size_t really the best bet here?
template <typename data_t, std::size_t t_size>
class array {
public:
    template <typename... args_t>
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


}} // namespace const_fmt::std


#endif


#endif // CONST_FMT_STDLIB_H
