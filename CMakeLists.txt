cmake_minimum_required(VERSION 3.16)
project(Cecilion LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${PROJECT_SOURCE_DIR}/include/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/Cecilion)
#add_subdirectory(${PROJECT_SOURCE_DIR}/src/Cecilion/Core/demo)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/App)


#add_subdirectory(${PROJECT_SOURCE_DIR}/src/Test)


