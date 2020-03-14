# These first lines tell the source folders to use the GLFW and GLEW directories




# The executablemust be added AFTER we have added directories, otherwise they won't be included.
#add_executable(Cecilion "")

# Create engine library
#add_library(Cecilion_engine STATIC "")

# Add the core library and OpenGL libraries to the engine
#target_link_libraries(Cecilion_engine Cecilion_Core glew32s.lib glfw3 gdi32  opengl32)
# Include core directory so we get all the header files.
#include_directories(${PROJECT_SOURCE_DIR}/src/core)

add_subdirectory(core)




#add_executable(Cecilion app/src/main.cpp)