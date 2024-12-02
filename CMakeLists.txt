# Specify the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(ASCII3DRenderer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Add Homebrew paths for Apple Silicon or Intel Mac
# if(APPLE)
#     if(EXISTS "/opt/homebrew/")
#         list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/")
#     elseif(EXISTS "/usr/local/")
#         list(APPEND CMAKE_PREFIX_PATH "/usr/local/")
#     endif()
# endif()

# Find and link ncurses
find_package(Curses REQUIRED)
find_package(glm REQUIRED)

# Add the source directory
add_subdirectory(src)
