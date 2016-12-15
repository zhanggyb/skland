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

## Build on ArchLinux

### Install prerequisites

