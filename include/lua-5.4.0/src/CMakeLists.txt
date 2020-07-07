file(GLOB Lua_core *.c)
add_library(Lua STATIC ${Lua_core})
#target_sources(Lua PUBLIC ${Lua_core})
#target_sources(Lua PUBLIC ${PROJECT_SOURCE_DIR}/include/ImGui/imgui.cpp
#        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_draw.cpp
#        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_widgets.cpp
#        ${PROJECT_SOURCE_DIR}/include/ImGui/imgui_demo.cpp
#        #PUBLIC imgui_impl_opengl3.cpp imgui_impl_opengl3.h
#        )
target_include_directories(Lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})