include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

# Create main Cecilion library.
include(Cecilion_HEADERS.cmake)
add_library(Cecilion STATIC "")

#target_precompile_headers(Cecilion INTERFACE Cecilion.h)


# Include the Core directory as private and the engine as public.
target_include_directories(Cecilion PRIVATE Core)
# add the Core library to Cecilion.

add_subdirectory(Event)
add_subdirectory(Core)
target_link_libraries(Cecilion PUBLIC Cecilion_Core)
target_include_directories(Cecilion PRIVATE Core)
target_precompile_headers(Cecilion REUSE_FROM Cecilion_Core)    # Tell CMake to reuse precompiled headers from core

#target_include_directories(Cecilion PUBLIC ${Cecilion_Event_HEADERS})
#target_include_directories(Cecilion PRIVATE ${Cecilion_Core_HEADERS})

# run the CMakeLists file in Core.


#add_subdirectory(Utils)


