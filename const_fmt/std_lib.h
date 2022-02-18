#ifndef CONST_FMT_STD_LIB_H
#define CONST_FMT_STD_LIB_H


#ifdef CONST_FMT_NO_STD_LIB


#include <stdint.h>


namespace std {


using size_t = uint16_t;


// TODO: Is std::size_t really the best bet here?
template <typename data_t, std::size_t t_size>
class array {
public:
    // Constructors

    array() = default;
    array(const array& other) = default;
    array(array&& other) = default;

    // Operators

    array operator=(const array& other) = default;
    array operator=(array&& other) = default;

    // Element access

    data_t& operator[](std::size_t index) {
        return m_data[index];
    }
    const data_t& operator[](std::size_t index) const {
        return m_data[index];
    }

    // Iterators

    using iterator       = data_t*;
    using const_iterator = const data_t*;

    iterator begin() {
        return &(m_data[0]);
    }
    iterator end() {
        return (&(m_data[t_size-1]) + 1);
    }

    const iterator cbegin() const {
        return &(m_data[0]);
    }
    const iterator cend() const {
        return (&(m_data[t_size-1]) + 1);
    }

private:
    data_t m_data[t_size];
};


} // namespace std


#endif


#endif // CONST_FMT_STD_LIB_H
