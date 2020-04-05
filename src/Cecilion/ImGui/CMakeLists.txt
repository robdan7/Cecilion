# --- Add ImGui library ---
file(GLOB IMGUI_CORE ${PROJECT_SOURCE_DIR}/include/ImGui/*.cpp)
add_library(ImGui_SOURCE STATIC ${IMGUI_CORE})
target_sources(ImGui_SOURCE PUBLIC ${PROJECT_SOURCE_DIR}/include/ImGui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_demo.cpp
        #PUBLIC imgui_impl_opengl3.cpp imgui_impl_opengl3.h
        )
target_include_directories(ImGui_SOURCE PUBLIC ${PROJECT_SOURCE_DIR}/include/ImGui)

add_library(ImGui_layer STATIC "")
target_sources(ImGui_layer PRIVATE ImGui_layer.cpp ImGui_build.cpp)
target_link_libraries(ImGui_layer PRIVATE OpenGL_glew Cecilion_Log ImGui_SOURCE)
target_include_directories(ImGui_layer PRIVATE ${CECILION_HEADER} ${PROJECT_SOURCE_DIR}/include/ImGui)