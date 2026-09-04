cmake_minimum_required(VERSION 3.20)
project(SummerEngine_Project CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================
# 1. PYTHON REFLECTION PARSER TARGET
# ==========================================

set(GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/generated")

# 1A. Run during CONFIGURE phase (Fixes the "Cannot find source file" error)
message(STATUS "Running Python Reflection Parser immediately for CMake Configuration...")
execute_process(
    COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/Tools/ReflectionParser.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# 1B. Run during BUILD phase (Catches live updates when you edit code and hit compile)
add_custom_target(GenerateReflection ALL
    COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/Tools/ReflectionParser.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating SPROPERTY() companion headers..."
)

# ==========================================
# 2. SOURCE GATHERING & PATH MAPPING
# ==========================================

# Glob the ORIGINAL files
file(GLOB_RECURSE RAW_ENGINE_SOURCES CONFIGURE_DEPENDS "src/SummerEngine/*.cpp" "src/SummerEngine/*.c")

# Map the original source paths to the GENERATED folder paths
set(ENGINE_SOURCES "")
# Compile the original sources directly. Generated files are companion headers only.
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
add_executable(SummerEngine ${ENGINE_SOURCES})

# Tell CMake to wait for the Python script to finish updating files before compiling
add_dependencies(SummerEngine GenerateReflection)

# ==========================================
# 3. AUTOMATIC INCLUDE PATH MAPPING
# ==========================================

# Glob the ORIGINAL headers
file(GLOB_RECURSE RAW_HEADERS CONFIGURE_DEPENDS "src/SummerEngine/*.h" "src/SummerEngine/*.hpp" "include/*.h" "include/*.hpp")

set(HEADER_DIRS "")
foreach(HEADER_PATH ${RAW_HEADERS})
    # Map the header path to the GENERATED folder so CMake looks at the mirrored files
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "${GEN_DIR}/" GEN_HEADER "${HEADER_PATH}")
    get_filename_component(DIR ${GEN_HEADER} DIRECTORY)
    list(APPEND HEADER_DIRS ${DIR})
endforeach()

if(HEADER_DIRS)
    list(REMOVE_DUPLICATES HEADER_DIRS)
endif()

# Point to the GENERATED directories instead of the raw ones
# Keep original include paths for source navigation and same-directory includes,
# then add generated companion paths.
file(GLOB_RECURSE ORIGINAL_HEADERS CONFIGURE_DEPENDS "include/*.h" "include/*.hpp" "src/*.h" "src/*.hpp")
set(ORIGINAL_HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/src")
foreach(ORIGINAL_HEADER ${ORIGINAL_HEADERS})
    get_filename_component(ORIGINAL_HEADER_DIR ${ORIGINAL_HEADER} DIRECTORY)
    list(APPEND ORIGINAL_HEADER_DIRS ${ORIGINAL_HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES ORIGINAL_HEADER_DIRS)

file(GLOB_RECURSE GENERATED_HEADERS CONFIGURE_DEPENDS "${GEN_DIR}/*.generated.h")
set(GENERATED_HEADER_DIRS "${GEN_DIR}")
foreach(GENERATED_HEADER ${GENERATED_HEADERS})
    get_filename_component(GENERATED_HEADER_DIR ${GENERATED_HEADER} DIRECTORY)
    list(APPEND GENERATED_HEADER_DIRS ${GENERATED_HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES GENERATED_HEADER_DIRS)

target_include_directories(SummerEngine PRIVATE 
    ${ORIGINAL_HEADER_DIRS}
    ${GENERATED_HEADER_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# ==========================================
# CENTRALIZED LOCAL /lib DEPENDENCY RESOLUTION
# ==========================================

set(LOCAL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# 1. GLFW Library (Looking inside /lib)
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