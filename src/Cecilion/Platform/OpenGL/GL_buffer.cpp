//
// Created by Robin on 2020-04-03.
//

#include "GL_buffer.h"
#include "OpenGL.h"
#include <Core/Log.h>
namespace Cecilion {
    uint32_t GL_shader_param_buffer::bindings = 0;

    class GL_Raw_buffer {
    public:

        static uint32_t calc_draw_type(Raw_buffer::Access_frequency frequency, Raw_buffer::Access_type type) {
            uint32_t draw_type = 0;
            // TODO there must be a better way of doing this and still make it dynamic and generic.
            switch(frequency) {
                case Raw_buffer::Access_frequency::STATIC:
                    switch (type) {
                        case Raw_buffer::Access_type::DRAW:
                            draw_type = GL_STATIC_DRAW;
                            break;
                        case Raw_buffer::Access_type::READ:
                            draw_type = GL_STATIC_READ;
                            break;
                        case Raw_buffer::Access_type::COPY:
                            draw_type = GL_STATIC_COPY;
                            break;
                    }
                    break;
                case Raw_buffer::Access_frequency::DYNAMIC:
                    switch (type) {
                        case Raw_buffer::Access_type::DRAW:
                            draw_type = GL_DYNAMIC_DRAW;
                            break;
                        case Raw_buffer::Access_type::READ:
                            draw_type = GL_DYNAMIC_READ;
                            break;
                        case Raw_buffer::Access_type::COPY:
                            draw_type = GL_DYNAMIC_COPY;
                            break;
                    }
                    break;
                case Raw_buffer::Access_frequency::STREAM:
                    switch (type) {
                        case Raw_buffer::Access_type::DRAW:
                            draw_type = GL_STREAM_DRAW;
                            break;
                        case Raw_buffer::Access_type::READ:
                            draw_type = GL_STREAM_READ;
                            break;
                        case Raw_buffer::Access_type::COPY:
                            draw_type = GL_STREAM_COPY;
                            break;
                    }
                    break;
            }
            return draw_type;
        }

        static uint32_t init(void *vertices, uint32_t size, uint32_t draw_type, uint32_t gl_type) {
            uint32_t ID = 0;
            glCreateBuffers(1, &ID);
            glBindBuffer(gl_type, reinterpret_cast<GLuint>(ID));
            glBufferData(gl_type, size, vertices, draw_type);
            glBindBuffer(gl_type, 0);
            return ID;
        }

        static void bind(uint32_t type, uint32_t ID) {
            glBindBuffer(type, reinterpret_cast<GLuint>(ID));
        }

        static void unbind(uint32_t type) {
            glBindBuffer(type, 0);
        }

        static void reset_buffer(uint32_t type, uint32_t ID, float *vertices, uint32_t size, uint32_t draw_type) {
            glBindBuffer(type, reinterpret_cast<GLuint>(ID));
            glBufferData(type, size, vertices, draw_type);
            glBindBuffer(type,0);
//        Raw_buffer::set_size(size);
        }

        static void set_sub_data(uint32_t type, uint32_t ID, float *vertices, uint32_t offset, uint32_t size) {
            glBindBuffer(type, reinterpret_cast<GLuint>(ID));
            glBufferSubData(type, offset, size, vertices);
            glBindBuffer(type,0);
        }

        static void resize_buffer(uint32_t type, uint32_t ID, uint32_t old_size, uint32_t new_size, uint32_t draw_type) {

            glBindBuffer(GL_COPY_READ_BUFFER, reinterpret_cast<GLuint>(ID));
            uint8_t data[new_size];
            glGetBufferSubData(GL_COPY_READ_BUFFER, 0, old_size < new_size ? old_size : new_size, &data);
            reset_buffer(type, ID, (float*)data, new_size, draw_type);

            glBindBuffer(GL_COPY_READ_BUFFER, 0);
        }

        static void delete_buffer(uint8_t ID) {
            glDeleteBuffers(1, reinterpret_cast<const GLuint *>(&ID));
        }
    };

    /// -------------- Vertex buffer --------------

    GL_vertex_buffer::GL_vertex_buffer(void* vertices, uint32_t size, Access_frequency frequency, Access_type type) : Vertex_buffer(size) {
        this->m_GL_draw_type = GL_Raw_buffer::calc_draw_type(frequency, type);
        this->m_buffer_ID = GL_Raw_buffer::init(vertices, size, this->m_GL_draw_type, GL_ARRAY_BUFFER);
        this->set_size(size);
    }


    void GL_vertex_buffer::bind() {
        GL_Raw_buffer::bind(GL_ARRAY_BUFFER, this->m_buffer_ID);
    }

    void GL_vertex_buffer::unbind() {
        GL_Raw_buffer::unbind(GL_ARRAY_BUFFER);
    }

    GL_vertex_buffer::~GL_vertex_buffer() {
        GL_Raw_buffer::delete_buffer(this->m_buffer_ID);
    }

    int GL_vertex_buffer::get_instance_divisor() {
        return this->m_divisor;
    }

    void GL_vertex_buffer::set_instance_divisor(int divisor) {
        this->m_divisor = divisor;
    }

    void GL_vertex_buffer::reset_buffer(float *vertices, uint32_t size) {
        GL_Raw_buffer::reset_buffer(GL_ARRAY_BUFFER, this->m_buffer_ID, vertices, size, this->m_GL_draw_type);
        this->set_size(size);
    }

    void GL_vertex_buffer::set_sub_data(float *vertices, uint32_t offset,uint32_t size) {
        GL_Raw_buffer::set_sub_data(GL_ARRAY_BUFFER,this->m_buffer_ID, vertices, offset, size);
    }

    void GL_vertex_buffer::resize_buffer(uint32_t size) {
        GL_Raw_buffer::resize_buffer(GL_ARRAY_BUFFER, this->m_buffer_ID, this->get_size(), size,this->m_GL_draw_type);
        this->set_size(size);
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reinterpret_cast<GLuint>(this->m_buffer_ID));
    }

    void GL_index_buffer::unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    /// -------------- Uniform buffer --------------

    void GL_shader_param_buffer::bind() {
        GL_Raw_buffer::bind(GL_UNIFORM_BUFFER, this->m_buffer_ID);
    }

    void GL_shader_param_buffer::unbind() {
        GL_Raw_buffer::unbind(GL_UNIFORM_BUFFER);
    }

    void GL_shader_param_buffer::reset_buffer(float *vertices, uint32_t size) {
        GL_Raw_buffer::reset_buffer(GL_UNIFORM_BUFFER, this->m_buffer_ID, vertices, size, this->m_GL_draw_type);
        this->set_size(size);
    }

    void GL_shader_param_buffer::set_sub_data(float *vertices, uint32_t offset, uint32_t size) {
        GL_Raw_buffer::set_sub_data(GL_UNIFORM_BUFFER,this->m_buffer_ID, vertices, offset, size);
    }

    void GL_shader_param_buffer::resize_buffer(uint32_t size) {
        GL_Raw_buffer::resize_buffer(GL_UNIFORM_BUFFER, this->m_buffer_ID, this->get_size(), size,this->m_GL_draw_type);
        this->set_size(size);
    }

    GL_shader_param_buffer::GL_shader_param_buffer(void *vertices, uint32_t size,
                                                   Raw_buffer::Access_frequency frequency,
                                                   Raw_buffer::Access_type type) : Shader_param_buffer(size){
        this->m_GL_draw_type = GL_Raw_buffer::calc_draw_type(frequency, type);
        this->m_buffer_ID = GL_Raw_buffer::init(vertices, size, this->m_GL_draw_type, GL_UNIFORM_BUFFER);
        this->m_binding_point = bindings++;
        glBindBufferBase(GL_UNIFORM_BUFFER, this->m_binding_point, this->m_buffer_ID);
    }

    GL_shader_param_buffer::GL_shader_param_buffer(uint32_t size,
            Raw_buffer::Access_frequency frequency,
            Raw_buffer::Access_type type) :
            GL_shader_param_buffer(nullptr, size, frequency, type) {
    }



}

