add_subdirectory(src)

add_executable(App src/main.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src/core)
target_link_libraries(App Cecilion_Core)