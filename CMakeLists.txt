# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(Crocodile)
set(CMAKE_CXX_STANDARD 14)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find raylib via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include     # Custom headers
    ${RAYLIB_INCLUDE_DIRS}          # Raylib headers
)

find_library(EGL_LIB EGL)
find_library(GLES_LIB GLESv2)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${RAYLIB_LIBRARY_DIRS})

# Create the executable
add_executable(build ${SOURCES})

# Link libraries
target_link_libraries(build
    ${RAYLIB_LIBRARIES}      # Link raylib from pkg-config
    GLESv2                   # Link libGLESv2
    EGL                      # Link libEGL
)

# Set rpath so libraries in 'lib' can be found at runtime
set_target_properties(build PROPERTIES
    INSTALL_RPATH "@executable_path/../lib"
    BUILD_WITH_INSTALL_RPATH ON
)

set_target_properties(build PROPERTIES
    INSTALL_RPATH "@loader_path/../lib"
    BUILD_WITH_INSTALL_RPATH ON
)

# Output binary directory
set_target_properties(build PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
