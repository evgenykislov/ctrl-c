/*
MIT License

Copyright (c) 2020 Evgeny Kislov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>
#include <condition_variable>
#include <mutex>

#include "ctrl-c.h"

int main()
{
  std::mutex wait_lock;
  std::condition_variable wait_var;
  const unsigned int kMaxCatches = 3;
  unsigned int catches = 0;

  unsigned int handler_id = CtrlCLibrary::SetCtrlCHandler([&catches, &wait_lock, &wait_var](enum CtrlCLibrary::CtrlSignal event) -> bool {
    switch (event) {
      case CtrlCLibrary::kCtrlCSignal:
        std::cout << "Caught Ctrl+C" << std::endl;
        std::lock_guard<std::mutex> locker(wait_lock);
        ++catches;
        wait_var.notify_all();
    }
    return true;
  });
  if (handler_id == CtrlCLibrary::kErrorID) {
    std::cerr << "Can't set ctrl+c handler" << std::endl;
    return 1;
  }

  std::cout << "Press Ctrl+C " << kMaxCatches << " times" << std::endl;
  std::unique_lock<std::mutex> locker(wait_lock);
  wait_var.wait(locker, [&catches, kMaxCatches](){
    return catches >= kMaxCatches;
  });
  std::cout << "All Ctrl+C's pressed" << std::endl;

  CtrlCLibrary::ResetCtrlCHandler(handler_id);

  return 0;
}
