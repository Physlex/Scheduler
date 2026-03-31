cmake -B build -S . -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake/clang-toolchain.cmake \
    -D CMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=1
