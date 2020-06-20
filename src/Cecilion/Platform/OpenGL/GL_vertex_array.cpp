#include "GL_vertex_array.h"
#include <Platform/OpenGL/OpenGL.h>
#include <Debug/Instrumentor.h>
namespace Cecilion {
    GL_vertex_array::GL_vertex_array() {
        glCreateVertexArrays(1, &this->m_render_ID);
    }
    void GL_vertex_array::bind() const {
        CECILION_PROFILE_FUNCTION();
        glBindVertexArray(this->m_render_ID);
    }

    void GL_vertex_array::unbind() const {
        glBindVertexArray(0);
    }

    /**
     * Append a vertex buffer to this vertex array.
     * @param vertex_buffer
     */
    void GL_vertex_array::add_vertex_buffer(const std::shared_ptr<Vertex_buffer>& vertex_buffer, uint32_t instance_divisor) {

        CORE_ASSERT(vertex_buffer->get_layout().get_elements().size(), "Vertex buffer layout is undefined!");
        glBindVertexArray(this->m_render_ID);
        vertex_buffer->bind();
        for(const auto& element: vertex_buffer->get_layout()) {
            /**
             * Since matrices take up four attribute locations, we need to set
             * one attrib pointer per column of the matrix. This means a 4x4 matrix will
             * take up 4 attribute locations.
             */
            int x = std::max((int)element.get_component_count(), 1) - 1;
            auto locations = (x-x%4)/4+1;

            /// Iterate through all the locations we need to create.
            for (int i = 0; i < locations; i++) {
                glEnableVertexAttribArray(m_current_index+i);
                glVertexAttribPointer(m_current_index+i,
                        element.get_base_component_count(),
                        GL_FLOAT,
                                      element.m_normalized ? GL_TRUE : GL_FALSE,
                        vertex_buffer->get_layout().get_stride(),
                        (const void*)(element.m_offset + element.m_base_component_size * i)
                        );
                glVertexAttribDivisor(m_current_index+i, instance_divisor);
            }
            m_current_index += locations;
        }
        glBindVertexArray(0);
        m_vertex_buffers.push_back(vertex_buffer);
    }

    /**
     * Set the index buffer that points to vertex indexes. There can only be one of these in a vertex
     * array.
     * @param index_buffer
     */
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

