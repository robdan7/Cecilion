#pragma once

#include <initializer_list>
#include <cstdint>
#include <Core/Log.h>
#include <Renderer/Renderer_API.h>

namespace Cecilion {
    enum class Shader_data {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };


    // TODO Move stuff to cpp files. I know the header files are pre compiled, but it's better that way.

    /**
     * This returns the complete size of a data type in floats.
     * @param type
     * @return
     */
    static uint32_t shader_data_count(Shader_data type) {
        switch(type) {
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

    /**
     * This returns the base size of a data type in floats. Matrices are typically
     * composed of multiple vectors, so this will return 3 or 4 instead of 9 and 16.
     * @param type
     * @return
     */
    static uint32_t base_shader_data_count(Shader_data type) {
        switch (type) {
            case Shader_data::Float:    return 1;
            case Shader_data::Float2:   return 2;
            case Shader_data::Float3:   return 3;
            case Shader_data::Float4:   return 4;
            case Shader_data::Mat3:     return 3;
            case Shader_data::Mat4:     return 4;
            case Shader_data::Int:      return 1;
            case Shader_data::Int2:     return 2;
            case Shader_data::Int3:     return 3;
            case Shader_data::Int4:     return 4;
            case Shader_data::Bool:     return 1;
        }
        CORE_ASSERT(false, "Unknown Shader data type!");
        return 0;
    }

    static char* get_shader_data_string(const Shader_data &type) {
        switch(Renderer_API::get_API()) {
            case Renderer_API::API::OpenGL:
                switch (type) {
                    case Shader_data::Float:    return "float";
                    case Shader_data::Float2:   return "vec2";
                    case Shader_data::Float3:   return "vec3";
                    case Shader_data::Float4:   return "vec4";
                    case Shader_data::Mat3:     return "mat3";
                    case Shader_data::Mat4:     return "mat4";
                    case Shader_data::Int:      return "int";
                    case Shader_data::Int2:     return "ivec2";
                    case Shader_data::Int3:     return "ivec3";
                    case Shader_data::Int4:     return "ivec4";
                    case Shader_data::Bool:     return "bool";
                }
                break;
            case Renderer_API::API::None:
                CORE_ASSERT(false, "Renderer::Buffer:: Could not find a render API!");
                break;
        }
        return "";
    }

    /**
     * Buffer elements are an abstract way of organising buffer layouts. One element
     * contains the data that defines the size, type and offset of one vertex attribute.
     */
    struct Buffer_element {
        std::string m_name;
        Shader_data m_type;
        char* m_string_representation;
        /** Total size in bytes. A component with multiple vectors will have a combined size.*/
        uint32_t m_size;
        /** Size per component part in bytes. A matrix component will have the byte size of one row/column. */
        uint32_t m_base_component_size;
        size_t m_offset;  /// Byte offset within one vertex.
        bool m_normalized;
        Buffer_element(Shader_data type, const std::string& name, bool normalized = false)
                : m_name(name) ,
                  m_type(type),
                  m_size(shader_data_count(type) << 2),
                  m_base_component_size(base_shader_data_count(type) << 2),
                  m_offset(0),
                  m_normalized(normalized),
                  m_string_representation(get_shader_data_string(type)) {

        }

        /**
         *
         * @return The total number of float-sized numbers in this component.
         */
        uint32_t get_component_count() const {
            return this->m_size >> 2;
        }

        /**
         * A matrix component has multiple rows and columns, this return the size of one row/column in floats.
         * @return
         */
        uint32_t get_base_component_count() const {
            return this->m_base_component_size >> 2;
        }
    };

    /**
     * The layout of a buffer defines the different data types that are
     * present withing one vertex. Remember that a vertex can not only contain
     * a position, but also color, texture coordinates and so much more. It is
     * completely up to the user to define what goes where.
     */
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
                element.m_offset = offset;
                offset += element.m_size;
                this->m_stride += element.m_size;
            }
        }
    private:
        uint32_t m_stride = 0;  /// The byte offset between two vertices.
        std::vector<Buffer_element> m_elements;
    };


    class Raw_buffer {
    public:
        enum class Access_frequency {
            STATIC,STREAM,DYNAMIC
        };
        enum class Access_type {
            DRAW, READ, COPY
        };

        /**
         * Binds this buffer. This function is directly taken from the OpenGL way of doing things.
         */
        virtual void bind() = 0;
        /**
         * Unbind this buffer. The opposite of binding.
         */
        virtual void unbind() = 0;

        virtual void reset_buffer(float* vertices,uint32_t size) = 0;
        virtual void set_sub_data(float* vertices,uint32_t offset, uint32_t size) = 0;
        virtual void resize_buffer(uint32_t size) = 0;

        uint32_t get_size() {return this->m_size;}
    protected:
        Raw_buffer(uint32_t size) : m_size(size){}
        void set_size(uint32_t size) {this->m_size = size;}
    private:
        uint32_t m_size;
    };

    class Vertex_buffer : public Raw_buffer {
    public:

        /**
         * Create a generic vertex buffer. This function will automatically
         * switch to the chosen renderer API when the program is compiled.
         *
         * define these features.
         * @param vertices - A pointer to an array of vertices.
         * @param size - The size of all vertices in bytes.
         * @return - A raw pointer to the newly created vertex buffer.
         */
        static Vertex_buffer* Create(void* vertices, uint32_t size, Raw_buffer::Access_frequency frequency, Raw_buffer::Access_type type);
        virtual ~Vertex_buffer() {}

        /**
         * Set the buffer layout, simple as that.
         * @param layout
         */
        virtual void set_layout(const Buffer_layout& layout) = 0;
        virtual Buffer_layout& get_layout() = 0;
        virtual int get_instance_divisor() = 0;
        virtual void set_instance_divisor(int divisor) = 0;

        virtual uint32_t get_ID() = 0;

    protected:
        Vertex_buffer(uint32_t size) : Raw_buffer(size) {}

    };

    /**
     * Class for storing global shader parameters that are not shader or
     * model specific. The OpenGL equivalent to this would be global uniforms.
     */
    class Shader_param_buffer : public Raw_buffer {
    protected:
        Shader_param_buffer(uint32_t size) : Raw_buffer(size) {}
    };



    class Index_buffer {
    public:
        /**
         * Create a generic index buffer. This function will automatically
         * switch to the chosen renderer API when the program is compiled.
         *
         * An index buffer is the buffer that contains the order in which vertices
         * will be drawn. This saves storage space if many vertices in the vertex
         * buffer are connected. It is highly recommended to use an index buffer
         * for meshes with more than one triangle.
         * @param indices - A pointer to an array of indices.
         * @param count - The size of all indices in bytes.
         * @return - A raw pointer to the newly created vertex buffer.
         */
        static Index_buffer* Create(uint32_t* indices, uint32_t count);
        virtual ~Index_buffer() {}
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual uint32_t get_count() const = 0;
    };
}

