include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

add_executable(App src/Sandbox.cpp)

#link Cecilion library, thats all we have to to on this side of the src folder :)
target_include_directories(App PRIVATE ${PROJECT_SOURCE_DIR}/src/Cecilion)
target_link_libraries(App PRIVATE Cecilion FBX Chaiscript Lua)