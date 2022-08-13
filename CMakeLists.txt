

# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project("MemoryContextDemo")

aux_source_directory(src DIR_SRCS)

add_executable(Demo  ${DIR_SRCS})
target_include_directories(Demo PRIVATE include)
