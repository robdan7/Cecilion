add_subdirectory(demo)

# Create demo library
add_library(Engine demo/demo.cpp)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# Add GLFW and GLEW to the engine core
target_link_libraries(Engine glew32s.lib glfw3 gdi32  opengl32)  # add glfw libraries