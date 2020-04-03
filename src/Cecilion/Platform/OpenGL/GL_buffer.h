#pragma once

#include <Renderer/Buffer.h>
namespace Cecilion {
    class GL_vertex_buffer : public Vertex_buffer {
    public:
        GL_vertex_buffer(float* vertices, uint32_t size);
        ~GL_vertex_buffer() override;
        void bind() override;
        void unbind() override;
    private:
        uint32_t m_buffer_ID;
    };


    class GL_index_buffer : public Index_buffer {
    public:
        GL_index_buffer(uint32_t* indices, uint32_t count);
        ~GL_index_buffer() override;
        void bind() override;
        void unbind() override;
        uint32_t get_count() const {return this->m_count;}
    private:
        uint32_t m_buffer_ID;
        uint32_t m_count;
    };

}


