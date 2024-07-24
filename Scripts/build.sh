#!/bin/sh

. scripts/env.sh

QT_BINARIES_PATH=$1

if [ -z "$QT_BINARIES_PATH" ]; then
    echo "The Path to Qt binaries is not specified as second argument"
else
    cmake -DCMAKE_BUILD_TYPE=Debug -DCUSTOM_CMAKE_PREFIX_PATH="$QT_BINARIES_PATH" -G Ninja -S "$SOURCE_DIR" -B "$DEBUG_BUILD_DIR"
    cmake -DCMAKE_BUILD_TYPE=Release -DCUSTOM_CMAKE_PREFIX_PATH="$QT_BINARIES_PATH" -G Ninja -S "$SOURCE_DIR" -B "$RELEASE_BUILD_DIR"
    cmake --build cmake-build-debug --target all -j6
    cmake --build cmake-build-release --target all -j6
fi