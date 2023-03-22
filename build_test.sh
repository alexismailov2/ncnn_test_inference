#! /bin/sh
export PATH=$PATH:"/c/mingw64/bin/"

cmake -Bbuild_mingw\
 -G "MinGW Makefiles"\
 -DCMAKE_MAKE_PROGRAM="/c/mingw64/bin/mingw32-make.exe"\
 -DCMAKE_BUILD_TYPE_INIT=Debug\
 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON\
 -DCMAKE_C_COMPILER_WORKS=ON\
 -DCMAKE_CXX_COMPILER_WORKS=ON
cmake --build build_mingw
./build_mingw/ncnn_test_inference.exe
