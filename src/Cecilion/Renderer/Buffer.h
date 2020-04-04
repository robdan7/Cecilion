#pragma once

#include <cstdint>
#include <Core/Log.h>

namespace Cecilion {
    enum class Shader_data {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t shader_data_size(Shader_data type) {
        switch(type) {
            case Shader_data::Float:    return 4;
            case Shader_data::Float2:   return 4*2;
            case Shader_data::Float3:   return 4*3;
            case Shader_data::Float4:   return 4*4;
            case Shader_data::Mat3:     return 4*3*3;
            case Shader_data::Mat4:     return 4*4*4;
            case Shader_data::Int:      return 4;
            case Shader_data::Int2:     return 4*2;
            case Shader_data::Int3:     return 4*3;
            case Shader_data::Int4:     return 4*4;
            case Shader_data::Bool:     return 1;
        }
        CORE_ASSERT(false, "Unknown Shader data type!");
        return 0;
    }

    struct Buffer_element {
        std::string name;
        Shader_data type;
        uint32_t size;
        size_t offset;
        bool normalized;
        Buffer_element(Shader_data type, const std::string& name, bool normalized = false)
            : name(name) , type(type), size(shader_data_size(type)), offset(0), normalized(normalized) {

        }
        Buffer_element() {}

        uint32_t get_component_count() const {
            switch (this->type) {
                case Shader_data::Float:    return 1;
                case Shader_data::Float2:   return 2;
                case Shader_data::Float3:   return 3;
                case Shader_data::Float4:   return 4;
                case Shader_data::Mat3:     return 3*3;
                case Shader_data::Mat4:     return 4*4;
                case Shader_data::Int:      return 1;
                case Shader_data::Int2:     return 2;
                case Shader_data::Int3:     return 3;
                case Shader_data::Int4:     return 4;
                case Shader_data::Bool:     return 1;
            }
            CORE_ASSERT(false, "Unknown Shader data type!");
            return 0;
        }
    };

    class Buffer_layout {
    public:
        Buffer_layout() {}
        Buffer_layout(const std::initializer_list<Buffer_element>& elements) : m_elements(elements){
            this->calculate_offset_and_stride();
        }

        inline const std::vector<Buffer_element>& get_elements() const {return this->m_elements;}
        std::vector<Buffer_element>::iterator begin() {return this->m_elements.begin();}
        std::vector<Buffer_element>::iterator end() {return this->m_elements.end();}
        std::vector<Buffer_element>::const_iterator begin() const {return this->m_elements.begin();}
        std::vector<Buffer_element>::const_iterator end() const {return this->m_elements.end();}
        uint32_t get_stride() {return this->m_stride;}
    private:
        void calculate_offset_and_stride() {
            size_t offset = 0;
            this->m_stride = 0;
            for (auto& element: this->m_elements) {
                element.offset = offset;
                offset += element.size;
                this->m_stride += element.size;
            }
        }
    private:
        uint32_t m_stride = 0;
        std::vector<Buffer_element> m_elements;
    };

    class Vertex_buffer {
    public:

        virtual ~Vertex_buffer() {}
        virtual void bind() = 0;
        virtual void unbind() = 0;
        static Vertex_buffer* Create(float* vertices, uint32_t size);
        virtual void set_layout(const Buffer_layout& layout) = 0;
        virtual Buffer_layout& get_layout() = 0;
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

