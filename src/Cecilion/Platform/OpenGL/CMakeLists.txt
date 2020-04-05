add_library(OpenGL_glew INTERFACE)

target_link_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/lib/Release/Win32)
target_include_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)

target_include_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include)
target_link_directories(OpenGL_glew INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/lib-mingw)

target_link_libraries(OpenGL_glew INTERFACE glew32s glfw3 gdi32 opengl32)
target_precompile_headers(OpenGL_glew INTERFACE OpenGL.h)

add_library(OpenGL_renderer STATIC GL_context.cpp GL_shader.cpp GL_buffer.cpp GL_vertex_array.cpp GL_renderer_API.cpp)
target_link_libraries(OpenGL_renderer PRIVATE OpenGL_glew Cecilion_Log glm_static)
target_include_directories(OpenGL_renderer PRIVATE ${CECILION_HEADER})
target_include_directories(OpenGL_renderer PUBLIC ${PROJECT_SOURCE_DIR}/include/glm)
target_precompile_headers(OpenGL_renderer PUBLIC GL_shader.h GL_buffer.h GL_context.h GL_renderer_API.h GL_vertex_array.h)

#target_include_directories(OpenGL INTERFACE ${CECILION_HEADER})
#target_precompile_headers(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include/GLFW/glfw3.h ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include/GL/glew.h)

# --- Add ImGui library ---
#file(GLOB IMGUI_CORE ${PROJECT_SOURCE_DIR}/include/ImGui/*.cpp)
#add_library(ImGui_SOURCE STATIC ${IMGUI_CORE})
#target_sources(ImGui_SOURCE PUBLIC ${PROJECT_SOURCE_DIR}/include/ImGui/imgui.cpp
#        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_draw.cpp
#        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_widgets.cpp
#        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_demo.cpp
#        #PUBLIC imgui_impl_opengl3.cpp imgui_impl_opengl3.h
#        )
#target_include_directories(ImGui_SOURCE PUBLIC ${PROJECT_SOURCE_DIR}/include/ImGui)

#add_library(ImGui STATIC "")
#target_sources(ImGui PRIVATE imgui_impl_opengl3.cpp imgui_impl_opengl3.h)
#target_link_libraries(ImGui PUBLIC ImGui_SOURCE PRIVATE OpenGL)
#target_precompile_headers(ImGui PUBLIC imgui_impl_opengl3.h)

add_library(ImGui INTERFACE)
