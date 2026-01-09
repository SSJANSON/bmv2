/*************************************************************************/
/* spdlog - an extremely fast and easy to use c++11 logging library.     */
/* Copyright (c) 2014 Gabi Melman.                                       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

// spdlog main header file.
// see example.cpp for usage example

#pragma once

#include "tweakme.h"
#include "common.h"
#include "logger.h"

#include <functional>
#include <memory>
#include <string>
#include <chrono>

namespace spdlog
{

// Return an existing logger or nullptr if it doesn't exist
std::shared_ptr<logger> get(const std::string& name);

//
// Set global formatting
//
void set_pattern(const std::string& format_string);
void set_formatter(formatter_ptr f);

//
// Set global logging level
//
void set_level(level::level_enum log_level);

//
// Async mode configuration
//
void set_async_mode(
    size_t queue_size,
    const async_overflow_policy overflow_policy =
        async_overflow_policy::block_retry,
    const std::function<void()>& worker_warmup_cb = nullptr,
    const std::chrono::milliseconds& flush_interval_ms =
        std::chrono::milliseconds::zero());

void set_sync_mode();

//
// Rotating file loggers
//
std::shared_ptr<logger> rotating_logger_mt(
    const std::string& logger_name,
    const std::string& filename,
    size_t max_file_size,
    size_t max_files,
    bool force_flush = false);

std::shared_ptr<logger> rotating_logger_st(
    const std::string& logger_name,
    const std::string& filename,
    size_t max_file_size,
    size_t max_files,
    bool force_flush = false);

//
// Daily file loggers
//
std::shared_ptr<logger> daily_logger_mt(
    const std::string& logger_name,
    const std::string& filename,
    int hour = 0,
    int minute = 0,
    bool force_flush = false);

std::shared_ptr<logger> daily_logger_st(
    const std::string& logger_name,
    const std::string& filename,
    int hour = 0,
    int minute = 0,
    bool force_flush = false);

//
// Console loggers
//
std::shared_ptr<logger> stdout_logger_mt(const std::string& logger_name);
std::shared_ptr<logger> stdout_logger_st(const std::string& logger_name);
std::shared_ptr<logger> stderr_logger_mt(const std::string& logger_name);
std::shared_ptr<logger> stderr_logger_st(const std::string& logger_name);

//
// Syslog logger (Linux only)
//
#ifdef __linux__
std::shared_ptr<logger> syslog_logger(
    const std::string& logger_name,
    const std::string& ident = "",
    int syslog_option = 0);
#endif

//
// Create loggers
//
std::shared_ptr<logger> create(
    const std::string& logger_name,
    sinks_init_list sinks);

template <class It>
std::shared_ptr<logger> create(
    const std::string& logger_name,
    const It& sinks_begin,
    const It& sinks_end);

template <typename Sink, typename... Args>
std::shared_ptr<logger> create(
    const std::string& logger_name,
    const Args&... args);

//
// Registry management
//
void register_logger(std::shared_ptr<logger> logger);
void drop(const std::string& name);
void drop_all();

///////////////////////////////////////////////////////////////////////////////
//
// Macros for source-location logging
//
///////////////////////////////////////////////////////////////////////////////

#ifdef SPDLOG_TRACE_ON
#define SPDLOG_TRACE(logger, ...) \
    (logger)->trace(__VA_ARGS__) << " (" << __FILE__ << " #" << __LINE__ << ")"
#else
#define SPDLOG_TRACE(logger, ...)
#endif

#ifdef SPDLOG_DEBUG_ON
#define SPDLOG_DEBUG(logger, ...) \
    (logger)->debug(__VA_ARGS__) << " (" << __FILE__ << " #" << __LINE__ << ")"
#else
#define SPDLOG_DEBUG(logger, ...)
#endif

} // namespace spdlog

#include "details/spdlog_impl.h"
