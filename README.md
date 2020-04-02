# ctrl-c
Crossplatform handler of Ctrl+C.
Library allows you to process Ctrl+C in custom handlers.
Supported OS: Windows, Linux.

Language: C++11
Library is used in form of source code: add two files (ctrl-c.h, ctrl-c.cpp from src folder) into your project.

Common notes:
You can add a few handlers for Ctrl+C processing: calling order is LIFO: first added handler is called at last.
Each handler should return bool value: true - to stop processing by other handlers; false - to continue processing by other handlers. (see notes for Windows below).

Library is thread-safe. Note: You shouldn't remove handler from handler code. It will cause deadlock.

Errors:
Adding a new handler can return error id (kErrorID) in case of system error or lack of memory.

Exceptions:
Library process its errors (and any bad_alloc at adding operation). In this case it will return error id (kErrorID).
Other exceptions (without of bad_alloc at adding operation) aren't processed - you should catch them by your code (for example, if your captured values in lambda throw exception during copying, etc).

Windows notes:
If all handlers returned false the previous handler will be called. For example, it can be called default OS handler and your process can be closed.

Linux notes:
Adding any handler will cause the previous handler will not work. If first adding caused error, previous handler will not work too - it sets DEFAULT handler.
Removing of all handlers causes setting DEFAULT handler for Ctrl+C.
