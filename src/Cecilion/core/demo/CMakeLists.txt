add_library(Demo STATIC "")
target_sources(Demo PRIVATE
        demo.cpp demo.h
        PUBLIC
        demo.h)
