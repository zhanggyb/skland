#!/bin/bash

PROJECT_PATH=$(dirname $(readlink -f "$0"))

PROJECT_PATH="${PROJECT_PATH%/*}"

DEPOT_TOOLS_SOURCE_DIR="${PROJECT_PATH}/third_party/depot_tools"
CHROMIUM_SOURCE_DIR="${PROJECT_PATH}/third_party/chromium"

echo -n "Checkout or sync chromium source code to ${CHROMIUM_SOURCE_DIR}?(y/n) "
read answer

if [ ${answer} = 'y' -o ${answer} = 'Y' ]; then
    echo -n
else
    exit 0
fi

if [ ! -d ${CHROMIUM_SOURCE_DIR} ]; then
	echo "${CHROMIUM_SOURCE_DIR} does not exist, create this folder"
	mkdir -p ${CHROMIUM_SOURCE_DIR}
fi

export PATH=${DEPOT_TOOLS_SOURCE_DIR}:$PATH

cd ${CHROMIUM_SOURCE_DIR}

#pwd
#echo $PATH

if [ ! -f .gclient ]; then
    # Use the fetch command in depot_tools
    fetch --nohooks chromium
else
    # If the dir exists, use 'gclient sync --nohooks' instead
    gclient sync --nohooks
fi

# Currently NOT needed: Once you've run ''install-build-deps' at least once, you
# can now run the Chromium-specific hooks, which will download additional
# binaries and other things you might need:

# cd "${CHROMIUM_SOURCE_DIR}/src"
# gclient runhooks

# cd "${CHROMIUM_SOURCE_DIR}/src"
# gn gen out/Default

# cd "${CHROMIUM_SOURCE_DIR}/src"
# ninja -C out/Default chrome