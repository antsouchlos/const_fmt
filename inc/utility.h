#ifndef LOGGER_UTILITY_H
#define LOGGER_UTILITY_H


namespace detail {


constexpr std::size_t const_pow(std::size_t base, int exponent) {
    if (exponent == 0)
        return 1;
    else
        return base * const_pow(base, exponent -1);
}


template <std::size_t N>
class ConstString {
public:
    constexpr ConstString(const char (&content)[N]) noexcept {
        std::copy(std::begin(content), std::end(content),
                  std::begin(m_content));
    }

    constexpr char operator[](std::size_t index) const noexcept {
        return m_content[index];
    }

    constexpr std::size_t size() const noexcept {
        return N - 1;
    }

    std::array<char, N> m_content;
};


template <std::size_t N>
ConstString(const char (&)[N]) -> ConstString<N>;


} // namespace detail


#endif // LOGGER_UTILITY_H
