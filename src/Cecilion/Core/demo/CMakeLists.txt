include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

add_library(Demo)
target_sources(Demo PRIVATE
        demo.cpp
        demo.h
        )

target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/include)
target_link_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include/glfw-3.3.2x86/lib-mingw)

target_link_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/lib/Release/Win32)
target_include_directories(Demo PUBLIC ${PROJECT_SOURCE_DIR}/include/glew-2.1.0/include)
target_precompile_headers(Demo PUBLIC demo.h)

target_include_directories(Demo PRIVATE ${Cecilion_Log_HEADERS})
target_link_libraries(Demo PRIVATE Cecilion_Log glfw3  glew32s gdi32  opengl32)


