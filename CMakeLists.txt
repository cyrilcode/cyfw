cmake_minimum_required(VERSION 3.5)
project(cyfw)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(tools/embed-resource)
include_directories(${EMBED_RES_INCLUDE_DIRS})

set(cyfw_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen
        ${EMBED_RES_INCLUDE_DIRS}
        )

add_subdirectory(lib/glad)
set(cyfw_extra_INCLUDE_DIR ${GLAD_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen/unsupported)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
endif()
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL " " FORCE)
set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw")

include_directories(${cyfw_INCLUDE_DIR} ${cyfw_extra_INCLUDE_DIR})

set(SOURCE_FILES
        src/cyfw/Window.cpp
        src/cyfw/Shader.cpp
        src/cyfw/Camera.cpp
        )

add_subdirectory(src/cyfw/Shaders)

add_library(cyfw STATIC ${SOURCE_FILES})
target_link_libraries(cyfw ${GLAD_LIBRARIES} glfw cyfw_shaders)

set(CYFW_INCLUDE_DIR ${cyfw_INCLUDE_DIR} PARENT_SCOPE)
