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

#include <atomic>
#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "base/iobserver.h"

namespace skylog {
namespace base {

template <typename ObserverMessageType>
class Observer : public IObserver<ObserverMessageType> {
 public:
  using ObserverMessage =
      typename IObserver<ObserverMessageType>::ObserverMessage;

  class Handler {
   public:
    virtual ~Handler() {}

    virtual void Handle(const ObserverMessage& message) = 0;
  };

  explicit Observer(Handler* handler)
      : service_()
      , work_(new boost::asio::io_service::work(service_))
      , thread_(new boost::thread(
            boost::bind(&boost::asio::io_service::run, &service_)))
      , handler_(handler)
      , is_running_(true) {}
  ~Observer() {
    Stop();
  }

  Observer(Observer&&) = default;
  Observer& operator=(Observer&&) = default;

  void Notify(const ObserverMessage& message) final {
    if (is_running_) {
      service_.post(boost::bind(&Handler::Handle, handler_, message));
    }
  }

 protected:
  void Stop() {
    bool expected_value = true;
    if (is_running_.compare_exchange_strong(expected_value, false)) {
      work_.reset();
      if (thread_) {
        thread_->join();
      }
    }
  }

 private:
  boost::asio::io_service service_;
  std::unique_ptr<boost::asio::io_service::work> work_;
  std::unique_ptr<boost::thread> thread_;

  Handler* handler_;
  std::atomic_bool is_running_;
};

}  // namespace base
}  // namespace skylog
