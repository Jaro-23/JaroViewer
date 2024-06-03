cmake_minimum_required(VERSION 3.17)

set(CMAKE_CPP_STANDARD 11)
set(CMAKE_CPP_STANDARD_REQUIRED True)

project(Tutorial LANGUAGES CXX)

add_executable(${PROJECT_NAME} tutorial.cpp)
