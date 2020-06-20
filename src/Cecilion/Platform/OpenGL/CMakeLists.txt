add_library(OpenGL_glew INTERFACE)

target_link_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/lib/Release/Win32)
target_include_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)

target_include_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include)
target_link_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/lib-mingw)

target_link_libraries(OpenGL_glew INTERFACE glew32s glfw3 gdi32 opengl32)
target_precompile_headers(OpenGL_glew INTERFACE OpenGL.h)

add_library(OpenGL_renderer STATIC GL_context.cpp GL_shader.cpp GL_buffer.cpp GL_vertex_array.cpp GL_renderer_API.cpp GL_Frustum_compute_query.cpp)
target_link_libraries(OpenGL_renderer PRIVATE OpenGL_glew Cecilion_Log glm_static)
target_include_directories(OpenGL_renderer PRIVATE ${CECILION_HEADER})
target_include_directories(OpenGL_renderer PUBLIC ${PROJECT_SOURCE_DIR}/include/glm)
target_precompile_headers(OpenGL_renderer PUBLIC GL_shader.h GL_buffer.h GL_context.h GL_renderer_API.h GL_vertex_array.h)



add_library(ImGui INTERFACE)
