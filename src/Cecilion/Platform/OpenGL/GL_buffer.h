#pragma once

#include <Renderer/Buffer.h>
namespace Cecilion {

    class GL_vertex_buffer : public Vertex_buffer {
    public:
        GL_vertex_buffer(void* vertices, uint32_t size, Access_frequency frequency, Access_type type);
//        GL_vertex_buffer(uint32_t size, Access_frequency frequency, Access_type type);
        ~GL_vertex_buffer() override;

        void bind() override;
        void unbind() override;

        void set_layout(const Buffer_layout &layout) override {this->m_layout = layout;}
        Buffer_layout &get_layout() override {return this->m_layout;}

        /**
         *
         * @return - The number of instances between two attributes. 0 represents a per vertex buffer.
         */
        int get_instance_divisor() override;

        /**
         * Set how many instances there should be between two attributes in the buffer.
         * There should be no need to set this value if the buffer contains per vertex values.
         * @param divisor
         */
        void set_instance_divisor(int divisor) override;

        void reset_buffer(float *vertices, uint32_t size) override;

        void resize_buffer(uint32_t size) override;

        void set_sub_data(float *vertices,uint32_t offset, uint32_t size) override;
        uint32_t get_ID() override {return this->m_buffer_ID;}

    private:
        Buffer_layout m_layout;
        int m_divisor = 0;
        uint32_t m_GL_draw_type;
        uint32_t m_buffer_ID;
//        int m_divisor = 0;
//        uint32_t m_GL_draw_type;
    };

    /**
     * Also known as uniform buffers.
     */
    class GL_constant_buffer : public Constant_buffer {
    public:
        GL_constant_buffer(void* vertices, uint32_t size, Access_frequency frequency, Access_type type);
        GL_constant_buffer(uint32_t size, Access_frequency frequency, Access_type type);
        void bind() override;

        void unbind() override;

        void reset_buffer(float *vertices, uint32_t size) override;

        void set_sub_data(float *vertices, uint32_t offset, uint32_t size) override;

        void resize_buffer(uint32_t size) override;
        inline uint32_t get_binding_point() {return this->m_binding_point;}
        uint32_t get_ID() override {return this->m_buffer_ID;}
    private:
        uint32_t m_GL_draw_type;
        uint32_t m_buffer_ID;
        uint32_t m_binding_point;
        static uint32_t bindings;
    };

    class GL_index_buffer : public Index_buffer {
    public:
        GL_index_buffer(uint32_t* indices, uint32_t count);
        ~GL_index_buffer() override;
        void bind() override;
        void unbind() override;
        uint32_t get_count() const override {return this->m_count;}
    private:
        uint32_t m_buffer_ID;
        uint32_t m_count;
    };
}


