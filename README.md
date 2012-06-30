libapply -- Higher-order programming for C
==========================================
This library is was inspired by
[Higher-order programming for C](http://vedantk.tumblr.com/post/26182854460/higher-order-programming-in-c),
a library which implements a functional-style `apply()` operation in C.

The original implementation only allowed functions which take `long` arguments.
This library allows passing any POD data type.

This implementation only support x86_64 at the moment, and includes
inline assembly to implement the AMD64 C calling convention.

Please fork this project if you are interested in adding i386 or
other implementations.

Limitations
-----------
Only x86-64 is supported at the moment.

Hacking
-------
Run the unit tests by doing `make test`.

Related Links
-------------
The [Hacker News discussion](http://news.ycombinator.com/item?id=4180537).
