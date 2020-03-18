# Create main Cecilion library.
add_library(Cecilion STATIC "")


# Include the Core directory as private and the engine as public.
#target_include_directories(Cecilion PRIVATE Core)

# add the Core library to Cecilion.
target_link_libraries(Cecilion PRIVATE Cecilion_Core Cecilion_Event Cecilion_Utils)

# run the CMakeLists file in Core.
add_subdirectory(Event)
add_subdirectory(Core)
add_subdirectory(Utils)


