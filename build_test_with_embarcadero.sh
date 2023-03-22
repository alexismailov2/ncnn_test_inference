#! /bin/sh
cmake -Bbuild_embarc\
 -G "Borland Makefiles"\
 -DCMAKE_MAKE_PROGRAM="/c/Program Files (x86)/Embarcadero/Studio/21.0/bin/make.exe"\
 -DCMAKE_C_COMPILER=bcc32x.exe\
 -DCMAKE_CXX_COMPILER=bcc32x.exe\
 -DCMAKE_BUILD_TYPE_INIT=Debug\
 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON\
 -DCMAKE_C_COMPILER_WORKS=ON\
 -DCMAKE_CXX_COMPILER_WORKS=ON
#sed -i 's:/:\\:g' ./build_embarc/CMakeFiles/ncnn_test_inference.dir/objects1.rsp
#cmake --build build_embarc
