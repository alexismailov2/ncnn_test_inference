cmake_minimum_required(VERSION 3.25)
project(ncnn_test_inference)

set(CMAKE_CXX_STANDARD 14)

#find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        #${OpenCV_INCLUDE_DIRS}
	C:\\ncnn_test_inference\\ncnn\\build_embarc\\install\\include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        #${OpenCV_LIBS}
	-LC:\\ncnn_test_inference\\ncnn\\build_embarc\\install\\lib ncnn.lib)
