#pragma once

#include <string>
#include <initializer_list>
#include <chrono>
#include <memory>
#include <exception>

// visual studio does not support noexcept yet
#ifndef _MSC_VER
#define SPDLOG_NOEXCEPT noexcept
#else
#define SPDLOG_NOEXCEPT throw()
#endif

namespace spdlog
{

class formatter;

namespace sinks
{
class sink;
}

// Common types across the lib
using log_clock = std::chrono::system_clock;
using sink_ptr = std::shared_ptr<sinks::sink>;
using sinks_init_list = std::initializer_list<sink_ptr>;
using formatter_ptr = std::shared_ptr<spdlog::formatter>;

namespace level
{
enum level_enum
{
    trace = 0,
    debug = 1,
    info = 2,
    notice = 3,
    warn = 4,
    err = 5,
    critical = 6,
    alert = 7,
    emerg = 8,
    off = 9
};

static const char* level_names[] = {
    "trace", "debug", "info", "notice", "warning",
    "error", "critical", "alert", "emerg", "off"
};

static const char* short_level_names[] = {
    "T", "D", "I", "N", "W", "E", "C", "A", "M", "O"
};

inline const char* to_str(level_enum l)
{
    return level_names[l];
}

inline const char* to_short_str(level_enum l)
{
    return short_level_names[l];
}

} // namespace level

enum class async_overflow_policy
{
    block_retry,
    discard_log_msg
};

class spdlog_ex : public std::exception
{
public:
    explicit spdlog_ex(const std::string& msg)
        : _msg(msg)
    {}

    const char* what() const SPDLOG_NOEXCEPT override
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

} // namespace spdlog
