cmake_minimum_required(VERSION 3.8)

project(gi_messager)

# C++11
set(CMAKE_CXX_STANDARD 11)

# 源码
FILE(GLOB_RECURSE DIR  src/*.cpp)
#add_executable(hello_world ${DIR})  设置源码列表后，获得可执行文件这一步就很方便了，不用巴拉巴拉写一长串源码文件； 这里只是说明下，不是这句真写在这个地方

include_directories(include)

add_library(model STATIC ${DIR}) # 生成libmodel.a