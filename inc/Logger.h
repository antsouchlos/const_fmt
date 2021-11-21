#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H


#include <array>
#include <iostream>
#include <tuple>

#include <format.h>
#include <ConstString.h>


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
    void log(args_t... args) {
        const auto formatted_msg = format<msg>(args...);

        for (const auto& c : formatted_msg) {
            m_output_policy.write(c);
        }
    }

private:
    output_policy_t& m_output_policy;
};


#endif // LOGGER_LOGGER_H
