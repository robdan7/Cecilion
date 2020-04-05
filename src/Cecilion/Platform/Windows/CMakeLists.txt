include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

# --- Add GL library ---
#add_library(OpenGL INTERFACE)
#target_include_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include)
#target_link_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/lib-mingw)

#target_link_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/lib/Release/Win32)
#target_include_directories(OpenGL INTERFACE ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)

#target_link_libraries(OpenGL INTERFACE glfw3  glew32s gdi32  opengl32)


add_library(Windows_library STATIC "")
target_sources(Windows_library PUBLIC W_window.cpp W_window.h Windows_input.cpp)
target_include_directories(Windows_library PRIVATE ${CECILION_HEADER})

#target_include_directories(Windows_library PUBLIC ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include)
#target_link_directories(Windows_library PUBLIC ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/lib-mingw)

#target_link_directories(Windows_library PUBLIC ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/lib/Release/Win32)
#target_include_directories(Windows_library PUBLIC ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)

#target_link_libraries(Windows_library PUBLIC glfw3  glew32s gdi32  opengl32 PRIVATE Cecilion_Log)
target_link_libraries(Windows_library PRIVATE Cecilion_Log OpenGL_glew Cecilion_input)
target_precompile_headers(Windows_library PUBLIC W_window.h)

