#!/bin/bash

PROJECT_PATH=$(dirname $(readlink -f "$0"))

PROJECT_PATH="${PROJECT_PATH%/*}"

DEPOT_TOOLS_SOURCE_DIR="${PROJECT_PATH}/third_party/depot_tools"
CHROMIUM_SOURCE_DIR="${PROJECT_PATH}/third_party/chromium"

if [ ! -d ${CHROMIUM_SOURCE_DIR} ]; then
	echo "${CHROMIUM_SOURCE_DIR} does not exist, create this folder"
	mkdir -p ${CHROMIUM_SOURCE_DIR}
fi

export PATH=${DEPOT_TOOLS_SOURCE_DIR}:$PATH

cd ${CHROMIUM_SOURCE_DIR}

#pwd
#echo $PATH

# Use the fetch command in depot_tools
fetch --nohooks chromium

# If the dir exists, use 'gclient sync --nohooks' instead

# Currently NOT needed: Once you've run ''install-build-deps' at least once, you
# can now run the Chromium-specific hooks, which will download additional
# binaries and other things you might need:

# cd "${CHROMIUM_SOURCE_DIR}/src"
# gclient runhooks

# cd "${CHROMIUM_SOURCE_DIR}/src"
# gn gen out/Default

# cd "${CHROMIUM_SOURCE_DIR}/src"
# ninja -C out/Default chrome