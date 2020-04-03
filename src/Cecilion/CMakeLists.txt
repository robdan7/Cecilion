include(Cecilion_HEADERS.cmake)

add_subdirectory(Platform)
add_subdirectory(Core)
add_subdirectory(Event)
add_subdirectory(ImGui)
add_subdirectory(Renderer)


add_library(Cecilion STATIC "")
target_link_libraries(Cecilion PUBLIC Cecilion_Core Cecilion_Log Platform Cecilion_Event ImGui_layer glm_static)
target_include_directories(Cecilion PUBLIC ${CECILION_HEADER})  # Let consumers include cecilion from here instead.




