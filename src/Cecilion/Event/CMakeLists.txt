include(${PROJECT_SOURCE_DIR}/src/Cecilion/Cecilion_HEADERS.cmake)
add_library(Cecilion_Event STATIC Event_actor_mt.cpp Event_actor_st.cpp Event_system.cpp)
#target_precompile_headers(Cecilion_Event PUBLIC
#        Event_system.h
#        Async_inbox.h
#        I_Event_actor.h
#        Event_message.h
#        Inbox_entry_container.h
#        Buffered_inbox.h)
target_include_directories(Cecilion_Event PRIVATE ${CECILION_HEADER})
target_link_libraries(Cecilion_Event PRIVATE Cecilion_Log Cecilion_Utils)

