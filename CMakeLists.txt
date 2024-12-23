# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(Crocodile)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
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
    /opt/homebrew/opt/llvm/include/c++/v1
    /opt/homebrew/opt/llvm/include
)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${RAYLIB_LIBRARY_DIRS})
link_directories(/opt/homebrew/opt/llvm/lib)

# Set RPATH for runtime library resolution
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#set(CMAKE_INSTALL_RPATH "@loader_path")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Create the executable
add_executable(build ${SOURCES})

# Link libraries
target_link_libraries(build
    ${RAYLIB_LIBRARIES}      # Link raylib from pkg-config
    GLESv2                   # Link libGLESv2
    EGL                      # Link libEGL
)

# Set runtime search paths for libraries
set_target_properties(build PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    INSTALL_RPATH "@executable_path/../lib"
    BUILD_WITH_INSTALL_RPATH ON
    LINK_FLAGS "-Wl,-rpath,@executable_path/../lib"
)
