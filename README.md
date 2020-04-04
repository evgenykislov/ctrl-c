# Ctrl+C

Crossplatform source code (C++11) to handle Ctrl+C event in custom functions.
Supports Windows, Linux and Mac OS X.

## Getting Started

To catch Ctrl+C event/signal you should call:

    unsigned int CtrlCLibrary::SetCtrlCHandler(std::function<bool(enum CtrlSignal)> handler);
handler - custom handler;

**Return**:
Returns handler identifier, or CtrlCLibrary::kErrorID in case of error.

To remove handler you should call:

    void CtrlCLibrary::ResetCtrlCHandler(unsigned int id);
 id - handler identifier, returned by CtrlCLibrary::SetCtrlCHandler.

### Installing

You should copy source files (src/ctrl-c.h, src/ctrl-c.cpp) to your project.

## Example of usage

Source file test/main.cpp contains example of usage Ctrl+C code.
You can compile the example by your favourite C++ compiler.

## Authors

**Evgeny Kislov** - [evgenykislov.com](https://evgenykislov.com), [github/evgenykislov](https://github.com/evgenykislov)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Notes

You can add a few handlers for Ctrl+C processing. All handlers will be called in LIFO order: first added handler will be called at last.
Each handler should return bool value:

* *true* - to stop processing by other handlers;
* *false* - to continue processing by other handlers (also, see notes for Windows below).

Library is thread-safe. *Warning*: You shouldn't remove handler from handler code. It will cause deadlock.

####Errors####
Adding a new handler can return error id (kErrorID) in case of system error or lack of memory.

####Exceptions####
The code process its errors (and any bad_alloc into SetCtrlCHandler call). In this case it will return error identifier (kErrorID).
Other exceptions aren't processed - you should catch them by your code.

####Linux notes####
Adding any handler will cause a previous handler will not work. If first adding caused error, previous handler will not work too - OS will use DEFAULT handler.
Removing of all handlers causes setting DEFAULT handler for Ctrl+C.

#### Windows notes####
If all handlers return **false**, a previous handler will be called. It can be default OS handler and your process will be closed.
