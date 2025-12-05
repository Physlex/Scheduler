#!/bin/sh
# file: `install.sh`
#
# brief: This file implements an installation script for project initialization purposes.

echo "Installing scheduler dependencies..."

sudo apt-get install -y \
    gcc  \
    make
