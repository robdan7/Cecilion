# Create Core library
add_library(glad STATIC "")
target_sources(glad PUBLIC ${PROJECT_SOURCE_DIR}/include/glad/src/glad.c)

add_library(Cecilion_Core STATIC "")
target_sources(Cecilion_Core PRIVATE
        Application.cpp
        Log.cpp)

#glew32s.lib
#spdlog
target_link_libraries(Cecilion_Core glad Demo glfw3 gdi32  opengl32 )
#target_link_libraries(Cecilion_Core Demo)

# Add all sub directories. Make sure to set target_sources if you want library files to be included.
#add_subdirectory(libDemo)
add_subdirectory(demo)
