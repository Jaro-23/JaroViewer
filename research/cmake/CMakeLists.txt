cmake_minimum_required(VERSION 3.17)

project(Tutorial LANGUAGES CXX)

set(CMAKE_CPP_STANDARD 17)
set(CMAKE_CPP_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)
set(LIBS opengl32)

add_executable(${PROJECT_NAME} src/tutorial.cpp)
target_link_libraries(Tutorial ${LIBS})
