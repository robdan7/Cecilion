include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)
add_library(Cecilion_Event STATIC "")
target_sources(Cecilion_Event
        PRIVATE I_Event_actor.cpp Event_system.cpp PUBLIC I_Event_actor.h Event_message.h Event_system.h
        )
target_precompile_headers(Cecilion_Event PUBLIC I_Event_actor.h Event_system.h Event_message.h)
target_link_libraries(Cecilion_Event PRIVATE Cecilion_Log)
target_include_directories(Cecilion_Event PRIVATE ${Cecilion_Log_HEADERS})
