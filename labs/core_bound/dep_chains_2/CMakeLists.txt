cmake_minimum_required(VERSION 2.8.12)

project(lab)

# Disable SLP vectorization to isolate the effect of breaking a dependency chain.
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-slp-vectorize")


# 禁用 SLP 向量化以隔离打破依赖链的影响
# 不同编译器使用不同的选项
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-slp-vectorize")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-tree-slp-vectorize")
else()
    message(WARNING "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}. SLP vectorization may not be disabled.")
endif()


macro(remove_cxx_flag flag)
  string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endmacro()

# Clear the -O3 flag in the CMAKE_CXX_FLAGS_RELEASE, otherwise it will reenable SLP vectorization.
remove_cxx_flag("-O3")

string(REGEX MATCH "^(.*)[\\/]labs[\\/].*$" repo "${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_MATCH_1}/tools/labs.cmake)
