include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)

add_executable(App src/Sandbox.cpp
        src/Sandbox.h
        src/Serializable.cpp)

find_library(YAML yaml-cpp ${PROJECT_SOURCE_DIR}/include)

#link Cecilion library, thats all we have to to on this side of the src folder :)
target_include_directories(App PRIVATE ${PROJECT_SOURCE_DIR}/src/Cecilion ${PROJECT_SOURCE_DIR}/include/pfr_non_boost/ ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(App PRIVATE Cecilion ${YAML})