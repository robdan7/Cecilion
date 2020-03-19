include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

# --- Add spdlog library ---
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${PROJECT_SOURCE_DIR}/include/spdlog-1.x/include)
target_precompile_headers(spdlog INTERFACE ${PROJECT_SOURCE_DIR}/include/spdlog-1.x/include/spdlog/spdlog.h ${PROJECT_SOURCE_DIR}/include/spdlog-1.x/include/spdlog/sinks/stdout_color_sinks.h)

# --- Add log library ---
add_library(Cecilion_Log STATIC "")
target_sources(Cecilion_Log PRIVATE Log.cpp Log.h)
target_link_libraries(Cecilion_Log PUBLIC spdlog)
target_precompile_headers(Cecilion_Log PUBLIC Log.h)


# --- Add core library ---
add_library(Cecilion_Core STATIC "")
target_sources(Cecilion_Core
        PUBLIC
        Application.cpp
        Application.h
        Core.h
        Entry_point.h
        )
target_precompile_headers(Cecilion_Core PRIVATE Core.h)


add_subdirectory(demo)

# --- set include directories ---
# set include directories for Cecilion_Core.
# PUBLIC directories can be used directly by consumers, while PRIVATE will only be included in the target.
# I.e. Cecilion can include Cecilion_Event but not demo, which is private to the core.
target_include_directories(Cecilion_Core PUBLIC ${Cecilion_Event_HEADERS} PRIVATE demo)

# --- Link libraries ---
target_link_libraries(Cecilion_Core PUBLIC Cecilion_Log Demo PRIVATE Cecilion_Event )



