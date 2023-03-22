#! /bin/sh

cd ncnn

cmake -Bbuild_embarc\
 -G "Borland Makefiles"\
 -DCMAKE_MAKE_PROGRAM="/c/Program Files (x86)/Embarcadero/Studio/21.0/bin/make.exe"\
 -DCMAKE_C_COMPILER=bcc32x.exe\
 -DCMAKE_CXX_COMPILER=bcc32x.exe\
 -DCMAKE_BUILD_TYPE_INIT=Debug\
 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON\
 -DCMAKE_C_COMPILER_WORKS=ON\
 -DCMAKE_CXX_COMPILER_WORKS=ON\
 -DNCNN_BUILD_TOOLS=OFF\
 -DNCNN_BUILD_EXAMPLES=OFF\
 -DNCNN_BUILD_BENCHMARK=OFF\
 -DNCNN_AVX=OFF\
 -DNCNN_AVX2=OFF\
 -DNCNN_SSE2=OFF\
 -DNCNN_RUNTIME_CPU=OFF\
 -DNCNN_THREADS=OFF\
 -DNCNN_OPENMP=OFF\
 -DNCNN_XOP=OFF\
 -DNCNN_PIXEL_AFFINE=OFF\
 -DNCNN_INSTALL_SDK=ON
sed -i 's:/:\\:g' ./build_embarc/src/CMakeFiles/ncnn.dir/objects1.rsp
cmake --build build_embarc --target install
