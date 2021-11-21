#ifndef LOGGER_UTILITY_H
#define LOGGER_UTILITY_H


#include <cstdlib>


namespace detail {


constexpr std::size_t const_pow(std::size_t base, std::size_t pow) {
    if (pow == 0)
        return 1;
    else
        return base * const_pow(base, pow - 1);
}


} // namespace detail


#endif // LOGGER_UTILITY_H
