add_library(Cecilion_ECS STATIC ECS.cpp)
target_include_directories(Cecilion_ECS PRIVATE ${PROJECT_SOURCE_DIR}/src/Cecilion)
target_link_libraries(Cecilion_ECS PRIVATE Cecilion_Log)