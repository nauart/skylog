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

#include <memory>
#include <string>

#include "base/observer.h"
#include "base/observable.h"

int main() {
  std::shared_ptr<skylog::base::IObserver<std::string> > observer =
      std::make_shared<skylog::base::Observer<std::string> >(nullptr);
  std::shared_ptr<
      skylog::base::IObservable<skylog::base::IObserver<std::string> > >
      observable = std::make_shared<
          skylog::base::Observable<skylog::base::IObserver<std::string> > >();
  observable->AddObserver("observer", observer);
  observable->NotifyObservers("message");

  return 0;
}
