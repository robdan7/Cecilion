add_library(OpenGL INTERFACE)

target_link_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/lib/Release/Win32)
target_include_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)

target_include_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include)
target_link_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/lib-mingw)

target_link_libraries(OpenGL INTERFACE glew32s glfw3 gdi32 opengl32)
target_precompile_headers(OpenGL INTERFACE OpenGL.h)

add_library(GL_context STATIC GL_context.cpp)
target_link_libraries(GL_context PRIVATE OpenGL Cecilion_Log)
target_include_directories(GL_context PRIVATE ${CECILION_HEADER})

add_library(GL_shaders STATIC GL_shader.cpp)
target_link_libraries(GL_shaders PRIVATE Cecilion_Log OpenGL)
target_include_directories(GL_shaders PRIVATE ${CECILION_HEADER})

add_library(GL_buffers STATIC GL_buffer.cpp GL_vertex_array.cpp)
target_link_libraries(GL_buffers PRIVATE Cecilion_Log OpenGL)
target_include_directories(GL_buffers PRIVATE ${CECILION_HEADER})


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
