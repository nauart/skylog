/*
 * Copyright (c) 2017 Artem Nosach
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <cstdint>
#include <string>
#include <chrono>

namespace skylog {
namespace appender {

enum class LogLevel {
  LL_UNKNOWN = 0,
  LL_TRACE,
  LL_DEBUG,
  LL_INFO,
  LL_WARN,
  LL_ERROR
};

class LoggerMessage {
 public:
  LoggerMessage()
      : level_(LogLevel::LL_UNKNOWN)
      , time_()
      , thread_id_(0)
      , file_name_()
      , function_name_()
      , line_number_(0)
      , log_string_() {}

  LoggerMessage(const LogLevel level,
                const std::chrono::system_clock::time_point& time,
                const std::uint32_t thread_id,
                const std::string& file_name,
                const std::string& function_name,
                const std::uint32_t line_number,
                const std::string& log_string)
      : level_(level)
      , time_(time)
      , thread_id_(thread_id)
      , file_name_(file_name)
      , function_name_(function_name)
      , line_number_(line_number)
      , log_string_(log_string) {}

  LogLevel level() const {
    return level_;
  }
  const std::chrono::system_clock::time_point& time() const {
    return time_;
  }
  std::uint32_t thread_id() const {
    return thread_id_;
  }
  const std::string& file_name() const {
    return file_name_;
  }
  const std::string& function_name() const {
    return function_name_;
  }
  std::uint32_t line_number() const {
    return line_number_;
  }
  const std::string& log_string() const {
    return log_string_;
  }

 private:
  const LogLevel level_;
  const std::chrono::system_clock::time_point time_;
  const std::uint32_t thread_id_;
  const std::string file_name_;
  const std::string function_name_;
  const std::uint32_t line_number_;
  const std::string log_string_;
};

}  // namespace appender
}  // namespace skylog
