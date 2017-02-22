Build the source code
=====================

## Checkout source code

```shell
$ git clone --recursive https://github.com/zhanggyb/skland.git
```

## Build on Fedora 25

### Install prerequisites

```shell
$ sudo dnf install gcc-c++ cmake wayland-devel mesa-libwayland-egl-devel libwebp-devel libjpeg-turbo-devel expat-devel lua-devel zlib-devel freetype-devel giflib-devel mesa-libGL-devel mesa-libEGL-devel fontconfig-devel
```

Optional (to build documents):
```shell
$ sudo dnf install doxygen graphviz
```

### Build the source code

```shell
$ cd <the parent directory you checkout skland>/skland
$ mkdir build
$ cd build
$ cmake ..
$ make
```

This will build and generate examples in `bin/` and library in `lib/`.

Available Cmake options:

- `-DCMAKE_BUILD_TYPE=<value>`: value = 'Release' or 'Debug'
- `-DBUILD_UNIT_TEST=<value>`: value = 'On', 'True', 'Off' or 'False'

### Build the document

```shell
$ make doc
```

This will generate html document through doxygen in `html/`, open the
 `html/index.html` with your favorite web browser.

## Build on ArchLinux

### Install prerequisites

