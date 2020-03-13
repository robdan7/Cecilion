# These fist lines of code makes GLFW and GLEW available to the entire src folder:
#include GLFW
include_directories(${PROJECT_SOURCE_DIR}/include/GLFW/include)
link_directories(${PROJECT_SOURCE_DIR}/include/GLFW/lib-mingw)

#include GLEW
include_directories(${PROJECT_SOURCE_DIR}/include/GLEW/include)
link_directories(${PROJECT_SOURCE_DIR}/include/GLEW/lib/Release/Win32)


# Now all we have to do is tell CMake to build the app and core
add_subdirectory(app)
add_subdirectory(core)