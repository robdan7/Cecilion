add_library(Demo STATIC "")
target_sources(Demo PRIVATE
        ${PROJECT_SOURCE_DIR}/include/glad/src/glad.c
        ${PROJECT_SOURCE_DIR}/include/glad/include/glad/glad.h
        ${PROJECT_SOURCE_DIR}/include/glad/include/KHR/khrplatform.h
        demo.cpp demo.h
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include/glad/include/glad/glad.h
        ${PROJECT_SOURCE_DIR}/include/glad/include/KHR/khrplatform.h
        demo.h)
