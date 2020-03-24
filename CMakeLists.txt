cmake_minimum_required(VERSION 2.8)

project(Homework)

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

# helloworld
add_executable(Homework Homework.cc)
target_link_libraries(Homework ${CERES_LIBRARIES})

