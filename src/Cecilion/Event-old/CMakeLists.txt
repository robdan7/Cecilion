include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)
add_library(Cecilion_Event STATIC Event_actor_mt.cpp Event_system.cpp)
#target_precompile_headers(Cecilion_Event PUBLIC Event_actor.h Event_actor_mt.h Event_actor_st.h Event_system.h System_events.h)
target_include_directories(Cecilion_Event PRIVATE ${CECILION_HEADER})
target_link_libraries(Cecilion_Event PRIVATE Cecilion_Log Cecilion_Utils)

