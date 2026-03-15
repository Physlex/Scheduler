# This file implements a set of build alias' for the build system

build:
	ninja -C build

format:
	cmake --build build --target=format	

clean:
	rm -rf build/

clean-examples:
	rm -rf build/examples

.PHONY: build format clean
