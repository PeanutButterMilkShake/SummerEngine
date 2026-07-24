cmake_minimum_required(VERSION 3.20)
project(SummerEngine_Project CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Gather all source files (Engine files + main.cpp)
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "src/SummerEngine/*.cpp" "src/SummerEngine/*.c")

# 3. Create the Executable directly with all sources
add_executable(SummerEngine ${ENGINE_SOURCES} src/main.cpp)

# 2. AUTOMATIC INCLUDE PATH MAPPING
file(GLOB_RECURSE ALL_HEADERS CONFIGURE_DEPENDS "src/SummerEngine/*.h" "src/SummerEngine/*.hpp" "include/*.h" "include/*.hpp")
set(HEADER_DIRS "")
foreach(HEADER_PATH ${ALL_HEADERS})
    get_filename_component(DIR ${HEADER_PATH} DIRECTORY)
    list(APPEND HEADER_DIRS ${DIR})
endforeach()

if(HEADER_DIRS)
    list(REMOVE_DUPLICATES HEADER_DIRS)
endif()

target_include_directories(SummerEngine PRIVATE 
    ${HEADER_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# ==========================================
# PORTABLE DEPENDENCY RESOLUTION
# ==========================================

# 1. Find GLFW (Standard way via find_package, or fallback to relative/env paths)
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    # Fallback: Look inside a local vendor or third-party folder within the workspace
    # e.g., external/glfw or similar, or check an environment variable
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/lib/libglfw3.a")
        set(GLFW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/lib/libglfw3.a")
    else()
        message(FATAL_ERROR "GLFW3 not found! Install it via package manager or place it in a 'vendor' folder.")
    endif()
else()
    set(GLFW_LIBRARY glfw)
endif()

# 2. Steamworks SDK - Use a relative path from the workspace or an environment variable
# Recommendation: Place 'steam_api.a' inside a local 'lib' or 'vendor' folder in your project repository.
set(STEAM_API_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/steam_api.a")

if(NOT EXISTS "${STEAM_API_LIB}")
    # Check if user defined a global environment variable for the SDK
    if(DEFINED ENV{STEAM_SDK_PATH})
        set(STEAM_API_LIB "$ENV{STEAM_SDK_PATH}/redistributable_bin/steam_api.a")
    else()
        message(WARNING "steam_api.a not found at ${STEAM_API_LIB}. Steam integration may fail to link.")
    endif()
endif()

# 4. Link Dependencies directly to the Executable
target_link_libraries(SummerEngine PRIVATE 
    opengl32 
    gdi32
    ${GLFW_LIBRARY}
    ${STEAM_API_LIB}
)

target_compile_options(SummerEngine PRIVATE -O3)

target_link_options(SummerEngine PRIVATE 
        "-static"
        "-static-libgcc"
        "-static-libstdc++"
    )

# Define source and destination
set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

# ==========================================
# FIX: Always Copy Assets
# ==========================================

# 1. Create a custom target that runs on EVERY build because of 'ALL'
add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSET_SOURCE_DIR}" "${ASSET_DEST_DIR}"
    COMMENT "Force copying assets to build folder..."
)

# 2. Tell CMake to run this copy target BEFORE trying to build the engine
add_dependencies(SummerEngine CopyAssets)