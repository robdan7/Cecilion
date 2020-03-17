cmake_minimum_required(VERSION 3.15)
project(Cecilion)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# I don't know why these need to be here. All the other stuff is in the Cecilion folder.
#include GLFW
include_directories(${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x64/include)
link_directories(${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x64/lib-mingw-w64)

#include GLEW
#include_directories(${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)
#link_directories(${PROJECT_SOURCE_DIR}/include/glew-2.1.0/bin/Release/x64)
include_directories(${PROJECT_SOURCE_DIR}/include/glad/include)

include_directories(${PROJECT_SOURCE_DIR}/include/spdlog-1.x)
link_directories(${PROJECT_SOURCE_DIR}/include/spdlog-1.x/src)

# Tell CMake to build the App and Cecilion source code
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Cecilion)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/App)



