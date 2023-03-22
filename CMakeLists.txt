cmake_minimum_required(VERSION 3.25)
project(ncnn_test_inference)

set(CMAKE_CXX_STANDARD 14)

#find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        #${OpenCV_INCLUDE_DIRS}
        ncnn/build_host/install/include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        #${OpenCV_LIBS}
        ${CMAKE_CURRENT_SOURCE_DIR}/ncnn/build_host/install/lib/libncnn.a)
