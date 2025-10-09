cmake_minimum_required(VERSION 3.15)
project(GUSEVEEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use target_include_directories instead of include_directories
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/GUSEVE
    ${CMAKE_SOURCE_DIR}/include/GUSEVE/Core
    ${CMAKE_SOURCE_DIR}/include/GUSEVE/Scene
    ${CMAKE_SOURCE_DIR}/include/GUSEVE/Graphics
    ${CMAKE_SOURCE_DIR}/include/GUSEVE/Game
    ${CMAKE_SOURCE_DIR}/include/GUSEVE/Core/Utility
)

# Collect source files
file(GLOB_RECURSE CPP_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/src/GUSEVE/Core/glad.c")

add_executable(Engine 
    ${CPP_SOURCES}
    ${GLAD_SOURCE}
)

# Set properties
set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE C)
target_include_directories(Engine PRIVATE ${INCLUDE_DIRS})

# Find and link GLFW properly
find_package(OpenGL REQUIRED)

target_link_libraries(Engine PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    ${OPENGL_LIBRARIES}
    -static-libgcc
    -static-libstdc++
)

# Windows-specific libraries
if(WIN32)
    target_link_libraries(Engine PRIVATE gdi32 user32 kernel32)
endif()