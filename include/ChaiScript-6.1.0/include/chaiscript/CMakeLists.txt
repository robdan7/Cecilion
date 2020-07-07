add_library(Chaiscript INTERFACE)
target_precompile_headers(Chaiscript INTERFACE
        chaiscript_basic.hpp
        chaiscript.hpp
        chaiscript_defines.hpp
        chaiscript_stdlib.hpp
        chaiscript_threading.hpp)
target_include_directories(Chaiscript INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})