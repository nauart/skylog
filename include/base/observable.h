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

#include <string>
#include <map>
#include <mutex>

#include "base/iobservable.h"

namespace skylog {
namespace base {

template <typename ObserverType>
class Observable : public IObservable<ObserverType> {
 public:
  using ObserverMessagePointer =
      typename IObservable<ObserverType>::ObserverMessagePointer;
  using ObserverPointer = typename IObservable<ObserverType>::ObserverPointer;

  Observable() = default;
  ~Observable() = default;
  Observable(Observable&&) = default;
  Observable& operator=(Observable&&) = default;

  bool AddObserver(const std::string& name, ObserverPointer&& observer) final {
    std::unique_lock<std::mutex> lock(observers_map_mutex_);
    return observers_map_.emplace(name, std::move(observer)).second;
  }
  void RemoveObserver(const std::string& name) final {
    std::unique_lock<std::mutex> lock(observers_map_mutex_);
    observers_map_.erase(name);
  }
  void NotifyObservers(const ObserverMessagePointer& message) final {
    std::unique_lock<std::mutex> lock(observers_map_mutex_);
    for (auto& kv : observers_map_) {
      kv.second->Notify(message);
    }
  }

 private:
  std::map<std::string, ObserverPointer> observers_map_;
  std::mutex observers_map_mutex_;
};

}  // namespace base
}  // namespace skylog
