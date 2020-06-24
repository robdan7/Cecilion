include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

add_executable(Debug src/Debug.cpp)

#link Cecilion library, thats all we have to to on this side of the src folder :)
target_include_directories(Debug PRIVATE ${PROJECT_SOURCE_DIR}/src/Cecilion)
target_link_libraries(Debug PRIVATE Cecilion)