# Create main Cecilion library.
add_library(Cecilion STATIC "")


# Include the core directory as private and the engine as public.
target_include_directories(Cecilion PUBLIC engine PRIVATE core)

# add the core library to Cecilion.
target_link_libraries(Cecilion PRIVATE Cecilion_Core Cecilion_engine)

# run the CMakeLists file in core.
add_subdirectory(core)
add_subdirectory(engine)
