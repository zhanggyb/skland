#!/usr/bin/env sh

OS=$(lsb_release -si)
ARCH=$(uname -m | sed 's/x86_//;s/i[3-6]86/32/')
VER=$(lsb_release -sr)
PACKAGES=""

if [ "$OS" = "Ubuntu" ]; then
	PACKAGES="cmake \
		libwayland-dev \
		libxkbcommon-dev \
		libegl1-mesa-dev \
		libgl1-mesa-dev \
		libgles2-mesa-dev \
		libvulkan-dev \
		libjpeg-dev \
		libgif-dev \
		libfreetype6-dev \
		libfontconfig1-dev \
		libexpat1-dev \
		zlib1g-dev \
		libwebp-dev \
		liblua5.3-dev \
		libinput-dev \
		libavcodec-dev \
		libavformat-dev \
		libavdevice-dev \
		libavutil-dev \
		libavfilter-dev \
		libswscale-dev \
		libpostproc-dev \
		libswresample-dev \
		doxygen \
		graphviz"
	sudo apt install ${PACKAGES}
elif [ "$OS" = "Fedora" ]; then
    PACKAGES="gcc-c++ \
        cmake \
        wayland-devel \
        mesa-libwayland-egl-devel \
        libwebp-devel \
        libjpeg-turbo-devel \
        expat-devel \
        lua-devel \
        zlib-devel \
        freetype-devel \
        giflib-devel \
        mesa-libGL-devel \
        mesa-libEGL-devel \
        fontconfig-devel \
        doxygen \
        graphviz"
    sudo dnf install ${PACKAGES}
else
	echo "Unsupported Linux distribution!"
fi

