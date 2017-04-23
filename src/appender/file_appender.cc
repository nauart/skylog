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

#include "appender/file_appender.h"

#include <string>
#include <fstream>
#include <chrono>

#include "base/observer.h"

skylog::appender::FileAppender::FileAppender(const std::string& file_path)
    : base::Observer<AppenderMessage>(this), file_stream_(file_path) {}

skylog::appender::FileAppender::~FileAppender() {}

void skylog::appender::FileAppender::Handle(const AppenderMessage& message) {
  if (!file_stream_.is_open()) {
    return;
  }

  const std::time_t time_stamp =
      std::chrono::system_clock::to_time_t(message.time());
  const std::size_t time_buffer_size = 50;
  char time_buffer[time_buffer_size];
  std::strftime(
      time_buffer, time_buffer_size, "%x %X", std::localtime(&time_stamp));

  file_stream_ << message.level_string() << " [" << time_buffer << "] ["
               << "0x" << std::hex << message.thread_id() << "] "
               << message.file_name() << " " << message.function_name()
               << "::" << std::dec << message.line_number() << ": "
               << message.log_string() << "\n";
  file_stream_.flush();
}
