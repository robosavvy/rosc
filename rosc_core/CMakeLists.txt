cmake_minimum_required(VERSION 2.8.3)
project(rosc_core)

find_package(catkin REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  CFG_EXTRAS rosc_core_src.cmake
)  

include_directories(include ${catkin_INCLUDE_DIRS})
