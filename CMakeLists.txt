cmake_minimum_required(VERSION 3.15)
project(Cecilion)
set(CMAKE_CXX_STANDARD 20)

# I don't know why these need to be here. All the other stuff is in the Cecilion folder.
#include GLFW
include_directories(${PROJECT_SOURCE_DIR}/include/GLFW/include)
link_directories(${PROJECT_SOURCE_DIR}/include/GLFW/lib-mingw)

#include GLEW
include_directories(${PROJECT_SOURCE_DIR}/include/GLEW/include)
link_directories(${PROJECT_SOURCE_DIR}/include/GLEW/lib/Release/Win32)


# Tell CMake to build the app and Cecilion source code
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Cecilion)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/app)



