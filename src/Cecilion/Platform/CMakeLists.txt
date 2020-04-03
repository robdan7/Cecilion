add_subdirectory(Windows)
add_subdirectory(OpenGL)

add_library(Platform INTERFACE)
target_link_libraries(Platform INTERFACE Windows_library OpenGL ImGui GL_context GL_buffers)