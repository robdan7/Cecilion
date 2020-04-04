#include "GL_vertex_array.h"
#include <Platform/OpenGL/OpenGL.h>
namespace Cecilion {
    GL_vertex_array::GL_vertex_array() {
        glCreateVertexArrays(1, &this->m_render_ID);
    }
    void GL_vertex_array::bind() const {
        glBindVertexArray(this->m_render_ID);
    }

    void GL_vertex_array::unbind() const {
        glBindVertexArray(0);
    }

    void GL_vertex_array::add_vertex_buffer(const std::shared_ptr<Vertex_buffer>& vertex_buffer) {

        CORE_ASSERT(vertex_buffer->get_layout().get_elements().size(), "Vertex buffer layout is undefined!");
        glBindVertexArray(this->m_render_ID);
        vertex_buffer->bind();
        uint32_t index = 0;
        for(const auto& element: vertex_buffer->get_layout()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,element.get_component_count(),GL_FLOAT,element.normalized?GL_TRUE:GL_FALSE, vertex_buffer->get_layout().get_stride(), (const void*)element.offset);
            index++;
            // TODO add automatic support for GL_FLOAT etc.
        }
        glBindVertexArray(0);
        m_vertex_buffers.push_back(vertex_buffer);
    }

    void GL_vertex_array::set_index_buffer(const std::shared_ptr<Index_buffer>& index_buffer) {
        glBindVertexArray(this->m_render_ID);
        index_buffer->bind();
        glBindVertexArray(0);
        this->m_index_buffer = index_buffer;
    }

    const std::vector<std::shared_ptr<Vertex_buffer>>& GL_vertex_array::get_vertex_buffer() const {
        return this->m_vertex_buffers;
    }

    const std::shared_ptr<Index_buffer>& GL_vertex_array::get_index_buffer() const {
        return this->m_index_buffer;
    }

    GL_vertex_array::~GL_vertex_array() {
        glDeleteVertexArrays(1, &this->m_render_ID);
    }
}

