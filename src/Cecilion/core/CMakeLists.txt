


# Create core library
add_library(Cecilion_Core STATIC "")
target_sources(Cecilion_Core PRIVATE
        Application.cpp
        Application.h
        PUBLIC
        Application.h)



target_link_libraries(Cecilion_Core Demo glew32s.lib glfw3 gdi32  opengl32)

# Add all sub directories. Make sure to set target_sources if you want library files to be included.
#add_subdirectory(libDemo)
add_subdirectory(demo)
