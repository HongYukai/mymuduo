cmake_minimum_required(VERSION 3.10)
project(mymuduo)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(RELEASE "Use Release Mode" ON)
configure_file(Logger_Helper.h.in Logger_Helper.h)

if(RELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# 定义参与编译的源文件 .表示所有源文件 SRC_LIST代表存放所有源文件的集合
aux_source_directory(. SRC_LIST)

# 编译生成动态库mymuduo
add_library(mymuduo SHARED ${SRC_LIST})

target_include_directories(mymuduo PUBLIC ${PROJECT_BINARY_DIR})
