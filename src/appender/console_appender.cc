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

#include "appender/console_appender.h"

#include <string>
#include <iostream>
#include <ctime>

#include "base/observer.h"

skylog::appender::ConsoleAppender::ConsoleAppender()
    : base::Observer<AppenderMessage>(this) {}

skylog::appender::ConsoleAppender::~ConsoleAppender() {}

void skylog::appender::ConsoleAppender::Handle(const AppenderMessage& message) {
  std::string level_string;
  switch (message.level()) {
    case LogLevel::LL_TRACE:
      level_string = "TRACE";
      break;
    case LogLevel::LL_DEBUG:
      level_string = "DEBUG";
      break;
    case LogLevel::LL_INFO:
      level_string = "INFO ";
      break;
    case LogLevel::LL_WARN:
      level_string = "WARN ";
      break;
    case LogLevel::LL_ERROR:
      level_string = "ERROR";
      break;
    default:
      return;
  }

  const std::time_t time_stamp = message.time();
  std::cout << level_string << " [" << std::asctime(std::localtime(&time_stamp))
            << "] ["
            << "0x" << std::hex << message.thread_id() << "] "
            << message.file_name() << " " << message.function_name()
            << "::" << std::dec << message.line_number() << ": "
            << message.log_string() << "\n";
  std::cout << std::flush;
}
