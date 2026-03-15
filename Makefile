# This file implements a set of build alias' for the build system

build:
	ninja -C build

format:
	cmake --build build --target=format	

clean:
	rm -rf build/

.PHONY: build format clean