New Signal/Slot Implementation in SkLand
========================================

## Overview

Originally [signals and slots](http://doc.qt.io/qt-5/signalsandslots.html) is a
language construct introduced in Qt for communication between objects. In other
language or framework, the similar technique is
called
[events and delegates](https://technet.microsoft.com/en-us/library/aa903294). Signal
and slot makes it easy to implement the Subject/Observer pattern while avoiding
boilerplate code. The concept is that objects (typically GUI widgets) can send
signals containing event information which can be received by other objects
using special functions known as slots. This is similar to C/C++ callbacks
(function pointers), but signal/slot system ensures the type-correctness of
callback arguments.

[SkLand](https://github.com/zhanggyb/skland) provides another built-in
signal/slot implementation which takes advantage of C++11 standard and fast C++
delegates.

**Note:** There's a standalone version
called [sigcxx](https://github.com/zhanggyb/sigcxx).

## Features

- Based on [fast C++ delegates](md_doc_delegates.html)
- Powered by variadic template in C++11
- Multicast
- Slot can be virtual or pure virtual
- Signal chaining
- Automatic disconnecting
- etc.

## Usage
