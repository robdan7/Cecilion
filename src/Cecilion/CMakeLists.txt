# Create main Cecilion library.
add_library(Cecilion STATIC "")


# Include the Core directory as private and the engine as public.
target_include_directories(Cecilion PRIVATE Core Event)

# add the Core library to Cecilion.
target_link_libraries(Cecilion PRIVATE Cecilion_Core)

# run the CMakeLists file in Core.
add_subdirectory(Core)
add_subdirectory(Event)

