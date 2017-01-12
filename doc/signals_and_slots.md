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

- Based on fast C++ delegates<sup id="a1">[1](#f1)</sup><sup>,</sup><sup
  id="a2">[2](#f2)</sup><sup>,</sup><sup id="a3">[3](#f3)</sup><sup>,</sup><sup
  id="a4">[4](#f4)</sup>
- Powered by variadic template in C++11
- Multicast
- Slot can be virtual or pure virtual
- Signal chaining
- Automatic disconnecting
- etc.

## Usage

----

<small>
<b id="f1">1</b>
[Member Function Pointers and the Fastest Possible C++ Delegates](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible),
author: Don Clugston. [↩](#a1)<br>
<b id="f2">2</b>
[The Impossibly Fast C++ Delegates](http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates), author: Sergey Ryazanov. [↩](#a2)<br>
<b id="f3">3</b>
[Fast C++ Delegate: Boost.Function 'drop-in' replacement and multicast](http://www.codeproject.com/Articles/18389/Fast-C-Delegate-Boost-Function-drop-in-replacement), author: JaeWook Choi. [↩](#a3)<br>
<b id="f4">4</b>
[CppEvents](http://code.google.com/p/cpp-events/), author: Nickolas V. Pohilets. [↩](#a4)
</small>
