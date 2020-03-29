add_library(ImGui_layer STATIC "")
target_sources(ImGui_layer PRIVATE ImGui_layer.cpp)
target_link_libraries(ImGui_layer PUBLIC ImGui PRIVATE OpenGL Cecilion_Log)
target_include_directories(ImGui_layer PRIVATE ${CECILION_HEADER})