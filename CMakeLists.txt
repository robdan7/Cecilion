cmake_minimum_required(VERSION 3.15)
project(Cecilion)
set(CMAKE_CXX_STANDARD 20)

#include GLFW
include_directories(${PROJECT_SOURCE_DIR}/include/GLFW/include)
link_directories(${PROJECT_SOURCE_DIR}/include/GLFW/lib-mingw)

#include GLEW
include_directories(${PROJECT_SOURCE_DIR}/include/GLEW/include)
link_directories(${PROJECT_SOURCE_DIR}/include/GLEW/lib/Release/Win32)

#include_directories(${PROJECT_SOURCE_DIR}/src)
# Tell CMake to build the source
add_subdirectory(${PROJECT_SOURCE_DIR}/src/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/app)


#target_sources(Cecilion PUBLIC src/app/src/main.cpp)


# Add the engine library. This will be included in the generated app exe file. Don't worry about copying DLLs :)
#target_link_libraries(Cecilion Cecilion_engine)



