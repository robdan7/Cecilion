//
// Created by Robin on 2020-04-03.
//

#include "GL_buffer.h"
#include "OpenGL.h"
namespace Cecilion {

    /// -------------- Vertex buffer --------------

    GL_vertex_buffer::GL_vertex_buffer(float *vertices, uint32_t size) {
        glCreateBuffers(1, &this->m_buffer_ID);
        glBindBuffer(GL_ARRAY_BUFFER, reinterpret_cast<GLuint>(&this->m_buffer_ID));
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GL_vertex_buffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, reinterpret_cast<GLuint>(&this->m_buffer_ID));

    }

    void GL_vertex_buffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GL_vertex_buffer::~GL_vertex_buffer() {
        glDeleteBuffers(1, &this->m_buffer_ID);
    }

    /// -------------- Index buffer --------------

    GL_index_buffer::GL_index_buffer(uint32_t *indices, uint32_t count) : m_count(count) {
        glCreateBuffers(1, &this->m_buffer_ID);
        this->bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    GL_index_buffer::~GL_index_buffer() {
        glDeleteBuffers(1, &this->m_buffer_ID);
    }

    void GL_index_buffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reinterpret_cast<GLuint>(&this->m_buffer_ID));
    }

    void GL_index_buffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

