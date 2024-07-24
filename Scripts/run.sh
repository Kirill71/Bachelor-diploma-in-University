#!/bin/sh

. scripts/env.sh

UNIT_TESTS=$1

OS=$(uname)

if [ "$OS" = "Darwin" ]; then
    cd "$MAC_OS_BIN_PATH" || exit
    if [ -z "$UNIT_TESTS" ]; then
        ./ips
    else
       ./unit-tests
    fi
elif [ "$OS" = "Linux" ]; then
    cd "$LINUX_OS_BIN_PATH" || exit
    if [ -z "$UNIT_TESTS" ]; then
        ./ips
    else
        ./unit-tests
    fi
else
    echo "Unknown OS: $OS"
fi