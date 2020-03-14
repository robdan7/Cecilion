


# Create core library
add_library(Cecilion_Core STATIC "")
target_link_libraries(Cecilion_Core Demo Cecilion_Core_libDemo glew32s.lib glfw3 gdi32  opengl32)

# Add all sub directories. Make sure to set target_sources if you want library files to be included.
add_subdirectory(libDemo)
add_subdirectory(demo)
