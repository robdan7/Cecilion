#pragma once

#include <cstdint>

namespace Cecilion {
    class Vertex_buffer {
    public:

        virtual ~Vertex_buffer() {}
        virtual void bind() = 0;
        virtual void unbind() = 0;
        static Vertex_buffer* Create(float* vertices, uint32_t size);
    };

    class Index_buffer {
    public:
        virtual ~Index_buffer() {}
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual uint32_t get_count() const = 0;
        static Index_buffer* Create(uint32_t* indices, uint32_t count);
    };
}

