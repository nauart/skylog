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

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

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
      , handler_(handler) {}
  ~Observer() {
    work_.reset();
    if (thread_) {
      thread_->join();
    }
  }

  Observer(Observer&&) = default;
  Observer& operator=(Observer&&) = default;

  void Notify(const ObserverMessage& message) final {
    service_.post(boost::bind(&Handler::Handle, handler_, message));
  }

 private:
  boost::asio::io_service service_;
  boost::scoped_ptr<boost::asio::io_service::work> work_;
  boost::scoped_ptr<boost::thread> thread_;
  Handler* handler_;
};

}  // namespace base
}  // namespace skylog
