include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)
add_library(Cecilion_Event STATIC I_Event_actor.cpp Async_inbox.cpp Event_system.cpp Inbox_entry_container.cpp I_Event_inbox.cpp Buffered_inbox.cpp)
#target_precompile_headers(Cecilion_Event PUBLIC
#        Event_system.h
#        Async_inbox.h
#        I_Event_actor.h
#        Event_message.h
#        Inbox_entry_container.h
#        Buffered_inbox.h)
target_link_libraries(Cecilion_Event PRIVATE Cecilion_Log)
target_include_directories(Cecilion_Event PRIVATE ${CECILION_HEADER})
