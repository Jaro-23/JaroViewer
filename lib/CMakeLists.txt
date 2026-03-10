cmake_minimum_required(VERSION 3.20)

project(JaroViewer VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch needed libraries
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.4.3
)
set(ASSIMP_BUILD_TESTS        OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL            OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT          ON  CACHE BOOL "" FORCE)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(glfw assimp glm stb)

# Build library

file(GLOB_RECURSE SOURCES 
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})
target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/glad.c")
set_source_files_properties("${PROJECT_SOURCE_DIR}/src/glad.c"
  PROPERTIES COMPILE_OPTIONS "-Wno-pedantic"
)


find_package(ZLIB REQUIRED)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}/headers
    ${PROJECT_SOURCE_DIR}/includes/headers
    ${glfw_SOURCE_DIR}/include
    ${assimp_SOURCE_DIR}/include 
    ${glm_SOURCE_DIR}
    ${stb_SOURCE_DIR}
  PRIVATE
    ${ZLIB_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    glfw
    assimp::assimp
    glm::glm
  PRIVATE
    ZLIB::ZLIB
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIB})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
