add_library(Cecilion_renderer STATIC
        Renderer.cpp
        Buffer.cpp
        Vertex_array.cpp
        Renderer_API.cpp
        Render_command.cpp
        Camera.cpp
        Shader.cpp
        Frustum_compute_query.cpp
        )
target_link_libraries(Cecilion_renderer PUBLIC glm_static PRIVATE Cecilion_Log OpenGL_renderer)
target_include_directories(Cecilion_renderer PRIVATE ${CECILION_HEADER})
target_include_directories(Cecilion_renderer PUBLIC ${PROJECT_SOURCE_DIR}/include/glm)
target_precompile_headers(Cecilion_renderer PUBLIC Render_command.h Renderer_API.h Renderer.h Vertex_array.h Camera.h Buffer.h)