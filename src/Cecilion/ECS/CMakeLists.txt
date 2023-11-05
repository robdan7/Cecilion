add_library(Cecilion_ECS STATIC ECS.cpp
        Entity.cpp
        Entity.h
        Component.h
)

find_library(YAML yaml-cpp ${PROJECT_SOURCE_DIR}/include)

target_include_directories(Cecilion_ECS PRIVATE ${PROJECT_SOURCE_DIR}/src/Cecilion PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Cecilion_ECS PRIVATE Cecilion_Log ${YAML})
target_precompile_headers(Cecilion_ECS PUBLIC ECS.h Filter.h Config.h Component_storage.h Component_iterator.h)