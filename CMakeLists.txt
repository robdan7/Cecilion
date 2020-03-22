cmake_minimum_required(VERSION 3.16)
project(Cecilion LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# I don't know why these need to be here. All the other stuff is in the Cecilion folder.
#include GLFW
#include_directories(${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include)
#link_directories(${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/lib-mingw)

#include GLEW
#include_directories(${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)
#link_directories(${PROJECT_SOURCE_DIR}/include/glew-2.1.0/bin/Release/Win32)
#include_directories(${PROJECT_SOURCE_DIR}/include/glad/include)

#include_directories(${PROJECT_SOURCE_DIR}/include/spdlog-1.x)
#link_directories(${PROJECT_SOURCE_DIR}/include/spdlog-1.x/src)

# Tell CMake to build the App and Cecilion source code
#add_subdirectory(${PROJECT_SOURCE_DIR}/src/Cecilion)
#add_subdirectory(${PROJECT_SOURCE_DIR}/src/App)

#add_subdirectory(include/spdlog-1.x/example)


#add_subdirectory(src/Cecilion/Event)
#add_subdirectory(src/Cecilion/Core)
#include_directories(${PROJECT_SOURCE_DIR}/src/Cecilion)
#include_directories(${PROJECT_SOURCE_DIR}/src/Event)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/Cecilion)
#add_subdirectory(${PROJECT_SOURCE_DIR}/src/Cecilion/Core/demo)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/App)



