include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)


add_library(Windows_library STATIC "")
target_sources(Windows_library PUBLIC W_window.cpp W_window.h Windows_input.cpp)
target_include_directories(Windows_library PRIVATE ${CECILION_HEADER})


target_link_libraries(Windows_library PRIVATE Cecilion_Log OpenGL_glew Cecilion_input Cecilion_Core)
#target_precompile_headers(Windows_library PUBLIC W_window.h)

