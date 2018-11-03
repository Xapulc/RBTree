cmake_minimum_required(VERSION 3.12)
project(RBTree)

set(CMAKE_CXX_STANDARD 11)

set(Sources RBTree.cpp treeTst.cpp)
set(Headers RBTree.h)

add_executable(superlib ${Sources} ${Headers})