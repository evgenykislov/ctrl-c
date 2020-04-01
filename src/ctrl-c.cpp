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

#include <algorithm>
#include <mutex>
#include <list>


#include <Windows.h>

#include "ctrl-c.h"

namespace CtrlCLibrary {

  struct HandlerItem {
    std::function<bool(enum CtrlSignal)> handler;
    unsigned int id;
  };

  bool SetSignalCatcher();
  void ResetSignalCatcher();

  const unsigned int kFirstID = 1;

  static std::mutex locker_;
  static std::list<HandlerItem> handlers_;
  static unsigned int last_id_ = kFirstID;

  unsigned int SetCtrlCHandler(std::function<bool(enum CtrlSignal)> handler) {
    bool reset_catcher_if_error = false;
    try {
      std::lock_guard<std::mutex> locker(locker_);
      if (handlers_.empty()) {
        if (!SetSignalCatcher()) {
          return kErrorID;
        }
        reset_catcher_if_error = true;
      }
      HandlerItem item;
      item.id = last_id_;
      item.handler = handler;
      handlers_.push_front(item);
      ++last_id_;
      return item.id;
    }
    catch (std::bad_alloc) {
      if (reset_catcher_if_error) {
        ResetSignalCatcher();
      }
    }
    catch (...) {
      if (reset_catcher_if_error) {
        ResetSignalCatcher();
      }
      throw;
    }
    return kErrorID;
  }

  void ResetCtrlCHandler(unsigned int id) {
    std::lock_guard<std::mutex> locker(locker_);
    auto iter = std::find_if(handlers_.begin(), handlers_.end(), [id](const HandlerItem& item){
      return item.id == id;
    });
    try {
      handlers_.erase(iter);
      if (handlers_.empty()) {
        ResetSignalCatcher();
      }
    }
    catch (...) {
      if (handlers_.empty()) {
        ResetSignalCatcher();
      }
      throw;
    }
  }

  bool EventFunction(enum CtrlSignal event) {
    std::lock_guard<std::mutex> locker(locker_);
    for (auto iter = handlers_.begin(); iter != handlers_.end(); ++iter) {
      if (iter->handler(event)) {
        return true;
      }
    }
    return false;
  }

  // Windows implementation

  BOOL WINAPI windows_catcher(_In_ DWORD dwCtrlType) {
    enum CtrlSignal event;
    switch (dwCtrlType) {
      case CTRL_C_EVENT:
        event = kCtrlCSignal;
        break;
      default:
        return FALSE;
    }
    return EventFunction(event) ? TRUE : FALSE;
  }

  bool SetSignalCatcher() {
    return SetConsoleCtrlHandler(windows_catcher, TRUE) != 0;
  }

  void ResetSignalCatcher() {
    SetConsoleCtrlHandler(windows_catcher, FALSE);
  }


};
