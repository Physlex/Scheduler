#!/bin/sh
# file: `install.sh`
#
# brief: This file implements an installation script for project initialization purposes.

echo "Installing project dependencies..."

sudo apt-get install -y \
    gcc  \
    make \
    cmake
