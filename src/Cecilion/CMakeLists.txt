include(Cecilion_HEADERS.cmake)

add_subdirectory(Core)
#add_subdirectory(Platform)
#add_subdirectory(Renderer)
#add_subdirectory(ImGui)
add_subdirectory(ECS)
add_subdirectory(Utils)
add_subdirectory(Event)


add_library(Cecilion STATIC "")
target_link_libraries(Cecilion PUBLIC Cecilion_Core  Cecilion_Log  glm_static Cecilion_ECS Cecilion_Utils boost_pfr)
target_include_directories(Cecilion PUBLIC ${CECILION_HEADER} ${INCLUDE_HEADER})  # Let consumers include cecilion from here instead.




