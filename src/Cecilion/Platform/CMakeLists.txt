add_subdirectory(Windows)

add_library(Platform INTERFACE)
target_link_libraries(Platform INTERFACE Windows_library)