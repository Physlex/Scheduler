#!/bin/bash

echo "Creating build environment..."

mkdir -p build

python3 -m venv .venv
source .venv/bin/activate

python3 -m pip install uv

uv > /dev/null
if [[ $? -ne 0 ]]; then
    echo "Failed to install uv build tool"
    return 1
fi

uv install

echo "Activation complete!"
