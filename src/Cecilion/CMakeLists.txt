# These first lines tell the source folders to use the GLFW and GLEW directories




# The executablemust be added AFTER we have added directories, otherwise they won't be included.
#add_executable(Cecilion "")

# Create engine library
#add_library(Cecilion_engine STATIC "")

# Add the core library and OpenGL libraries to the engine
#target_link_libraries(Cecilion_engine Cecilion_Core glew32s.lib glfw3 gdi32  opengl32)
# Include core directory so we get all the header files.
#include_directories(${PROJECT_SOURCE_DIR}/src/core)

# Create Cecilion library
add_library(Cecilion "")
target_sources(Cecilion PUBLIC Cecilion.h)
target_link_libraries(Cecilion Cecilion_Core)

# Add include files to the library.
#include_directories(Cecilion PUBLIC core)
target_include_directories(Cecilion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
