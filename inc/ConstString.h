#ifndef LOGGER_CONSTSTRING_H
#define LOGGER_CONSTSTRING_H


namespace detail {


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


#endif // LOGGER_CONSTSTRING_H
