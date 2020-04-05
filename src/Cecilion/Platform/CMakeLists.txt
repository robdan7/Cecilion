add_subdirectory(Windows)
add_subdirectory(OpenGL)

add_library(Platform INTERFACE)
target_link_libraries(Platform INTERFACE Windows_library OpenGL_glew ImGui OpenGL_renderer)