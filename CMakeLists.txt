cmake_minimum_required(VERSION 3.20)
project(SummerEngine_Project CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Gather all source files (Engine files + main.cpp)
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "src/SummerEngine/*.cpp" "src/SummerEngine/*.c")

# 2. Create the Executable directly with all sources
add_executable(SummerEngine ${ENGINE_SOURCES} src/main.cpp)

# 3. AUTOMATIC INCLUDE PATH MAPPING
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
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"   # <--- Added /lib so headers inside lib can be found if needed
)

# ==========================================
# CENTRALIZED LOCAL /lib DEPENDENCY RESOLUTION
# ==========================================

set(LOCAL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# 1. GLFW Library (Looking inside /lib)
# Adjust the filename if yours is named differently (e.g., glfw3.lib or libglfw3.a)
if(EXISTS "${LOCAL_LIB_DIR}/libglfw3.a")
    set(GLFW_LIBRARY "${LOCAL_LIB_DIR}/libglfw3.a")
elseif(EXISTS "${LOCAL_LIB_DIR}/glfw3.lib")
    set(GLFW_LIBRARY "${LOCAL_LIB_DIR}/glfw3.lib")
else()
    message(FATAL_ERROR "GLFW library not found inside ${LOCAL_LIB_DIR}!")
endif()

# 2. Steamworks SDK Library (Looking inside /lib)
if(EXISTS "${LOCAL_LIB_DIR}/steam_api.a")
    set(STEAM_API_LIB "${LOCAL_LIB_DIR}/steam_api.a")
elseif(EXISTS "${LOCAL_LIB_DIR}/steam_api.lib")
    set(STEAM_API_LIB "${LOCAL_LIB_DIR}/steam_api.lib")
else()
    message(WARNING "steam_api library not found in ${LOCAL_LIB_DIR}. Steam integration may fail to link.")
    set(STEAM_API_LIB "")
endif()

# ==========================================
# LINK LIBRARIES & COMPILER OPTIONS
# ==========================================

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

# ==========================================
# ASSET COPYING
# ==========================================

set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSET_SOURCE_DIR}" "${ASSET_DEST_DIR}"
    COMMENT "Force copying assets to build folder..."
)

add_dependencies(SummerEngine CopyAssets)