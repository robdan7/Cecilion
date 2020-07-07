include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

# --- Add spdlog library ---
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${PROJECT_SOURCE_DIR}/include/spdlog-1.x/include)
target_precompile_headers(spdlog INTERFACE ${PROJECT_SOURCE_DIR}/include/spdlog-1.x/include/spdlog/spdlog.h ${PROJECT_SOURCE_DIR}/include/spdlog-1.x/include/spdlog/sinks/stdout_color_sinks.h)

# --- Add log library ---
add_library(Cecilion_Log STATIC "")
target_sources(Cecilion_Log PUBLIC Log.cpp Log.h)
target_link_libraries(Cecilion_Log PUBLIC spdlog)
target_precompile_headers(Cecilion_Log PUBLIC Log.h)

# --- Add input library ---
add_library(Cecilion_input STATIC "")
target_sources(Cecilion_input PUBLIC Input.cpp)
target_link_libraries(Cecilion_input PRIVATE Cecilion_Log)
target_precompile_headers(Cecilion_input PUBLIC Input.h)
target_include_directories(Cecilion_input PRIVATE ${CECILION_HEADER})

# --- Add core library ---
add_library(Cecilion_Core STATIC "")
target_sources(Cecilion_Core
        PUBLIC
        Application.cpp
        Window.cpp
        ../Assets/Instanced_mesh.cpp
        )
target_include_directories(Cecilion_Core PRIVATE ${CECILION_HEADER})
target_link_libraries(Cecilion_Core PUBLIC FBX Cecilion_renderer Cecilion_Event PRIVATE Cecilion_Log Platform Cecilion_input )

# Only precompile PUBLIC headers if they are used outside of Cecilion_Core.
target_precompile_headers(Cecilion_Core PUBLIC Window.h Mouse_button_codes.h Keycodes.h Application.h Layers/Layer.h PRIVATE Layers/Layer_stack.h Layers/Layer_group.h)
