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
set(cyfw_INCLUDE_DIR ${cyfw_INCLUDE_DIR} ${GLAD_INCLUDE_DIRS})
set(cyfw_extra_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen/unsupported)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
endif()
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL " " FORCE)
set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw")

add_subdirectory(lib/video_capture)
set(cyfw_INCLUDE_DIR ${cyfw_INCLUDE_DIR} ${VIDCAP_INCLUDE_DIR})

add_subdirectory(lib/imgui)
set(cyfw_extra_INCLUDE_DIR ${cyfw_extra_INCLUDE_DIR} ${IMGUI_INCLUDE_DIRS})

add_library(imgui_glfw lib/imgui/examples/opengl3_example/imgui_impl_glfw_gl3.cpp)

include_directories(${cyfw_INCLUDE_DIR} ${cyfw_extra_INCLUDE_DIR})

set(SOURCE_FILES
        src/cyfw/Window.cpp
        src/cyfw/Shader.cpp
        src/cyfw/Camera.cpp
        src/cyfw/VideoGrabber.cpp
        src/cyfw/Texture.cpp
        src/cyfw/Logger.cpp
        src/cyfw/Gui.cpp
        )

add_subdirectory(src/cyfw/Shaders)

add_library(cyfw STATIC ${SOURCE_FILES})
target_link_libraries(cyfw ${GLAD_LIBRARIES}
        glfw cyfw_shaders ${VIDCAP_LIBRARIES}
        ${IMGUI_LIBRARIES} imgui_glfw ${IMGUI_DEMO_LIBRARY}
        )

set(CYFW_INCLUDE_DIR ${cyfw_INCLUDE_DIR} PARENT_SCOPE)
