add_library(Cecilion_renderer STATIC Renderer.cpp Buffer.cpp)
target_link_libraries(Cecilion_renderer PRIVATE Cecilion_Log GL_buffers)
target_include_directories(Cecilion_renderer PRIVATE ${CECILION_HEADER})