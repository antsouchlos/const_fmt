#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H


#include <array>
#include <iostream>
#include <tuple>

#include <parsing.h>
#include <utility.h>


/*
 *
 * Logger class
 *
 */


namespace detail {

template <typename T>
concept output_policy_c = requires(T t) {
    t.write('c');
};

} // namespace detail


template <detail::output_policy_c output_policy_t>
class Logger {

public:
    Logger(output_policy_t output_policy) : m_output_policy(output_policy) {
    }

    template <detail::ConstString msg, typename... args_t>
    void log(args_t...) {
        constexpr int len = get_output_len(msg);
        static_assert(len > 0, "Syntax error in log string");

        std::cout << "Computed Length: " << len << std::endl;

        for (unsigned i = 0; i < msg.size(); ++i) {
            m_output_policy.write(msg[i]);
        }
    }

private:
    output_policy_t& m_output_policy;
};


#endif // LOGGER_LOGGER_H
