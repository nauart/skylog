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

#include "appender/socket_appender.h"

#include <string>
#include <sstream>
#include <iostream>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "base/observer.h"

skylog::appender::SocketAppender::SocketAppender(
    const std::string& address, const std::string& service_name)
    : BaseObserver(this), service_(), socket_(service_) {
  Connect(address, service_name);
}

skylog::appender::SocketAppender::~SocketAppender() {
  BaseObserver::Stop();
  Disconnect();
}

void skylog::appender::SocketAppender::Handle(
    const AppenderMessagePointer& message) {
  const std::time_t time_stamp =
      std::chrono::system_clock::to_time_t(message->time());
  const std::size_t time_buffer_size = 50;
  char time_buffer[time_buffer_size];
  std::strftime(
      time_buffer, time_buffer_size, "%x %X", std::localtime(&time_stamp));

  boost::property_tree::ptree root;
  root.put("level", message->level_string());
  root.put("time", time_buffer);
  root.put("thread_id", message->thread_id());
  root.put("file_name", message->file_name());
  root.put("function_name", message->function_name());
  root.put("line_number", message->line_number());
  root.put("log_string", message->log_string());

  std::stringstream output_stream;
  boost::property_tree::write_json(output_stream, root);

  boost::system::error_code error;
  boost::asio::write(socket_, boost::asio::buffer(output_stream.str()), error);
}

void skylog::appender::SocketAppender::Connect(
    const std::string& address, const std::string& service_name) {
  boost::asio::ip::tcp::resolver resolver(service_);
  boost::asio::ip::tcp::resolver::query query(address, service_name);

  boost::system::error_code error;
  boost::asio::ip::tcp::resolver::iterator iterator =
      resolver.resolve(query, error);
  if (error) {
    std::cerr << "Socket appender cannot resolve the host: " << address << ":"
              << service_name << ". Error message: " << error.message() << "\n";
    return;
  }

  error = boost::asio::error::host_not_found;
  boost::asio::ip::tcp::resolver::iterator end;
  while (error && iterator != end) {
    socket_.close(error);
    socket_.connect(*iterator++, error);
  }

  if (error) {
    std::cerr << "Socket appender cannot connect to host: " << address << ":"
              << service_name << ". Error message: " << error.message() << "\n";
  }
}

void skylog::appender::SocketAppender::Disconnect() {
  boost::system::error_code error;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);
  socket_.close(error);

  if (error) {
    std::cerr << "Socket appender cannot disconnect from host."
              << " Error message: " << error.message() << "\n";
  }
}
