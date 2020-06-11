#pragma once

#include <cstdint>
#include <Core/Log.h>

namespace Cecilion {
    enum class Shader_data {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

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

    /**
     * Buffer elements are an abstract way of organising buffer layouts. One element
     * contains the data that defines the size, type and offset of one vertex attribute.
     */
    struct Buffer_element {
        std::string name;
        Shader_data type;
        /** Total size in bytes. A component with multiple vectors will have a combined size.*/
        uint32_t size;
        /** Size per component part in bytes. A matrix component will have the byte size of one row/column. */
        uint32_t base_component_size;
        size_t offset;  /// Byte offset within one vertex.
        bool normalized;
        Buffer_element(Shader_data type, const std::string& name, bool normalized = false)
            : name(name) , type(type), size(shader_data_count(type)<<2), base_component_size(base_shader_data_count(type)<<2), offset(0), normalized(normalized) {

        }
        Buffer_element() {}

        /**
         *
         * @return The total number of float-sized numbers in this component.
         */
        uint32_t get_component_count() const {
            return this->size>>2;
        }

        /**
         * A matrix component has multiple rows and columns, this return the size of one row/column in floats.
         * @return
         */
        uint32_t get_base_component_count() const {
            return this->base_component_size>>2;
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
                element.offset = offset;
                offset += element.size;
                this->m_stride += element.size;
            }
        }
    private:
        uint32_t m_stride = 0;  /// The byte offset between two vertices.
        std::vector<Buffer_element> m_elements;
    };

    class Vertex_buffer {
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
        static Vertex_buffer* Create(float* vertices, uint32_t size);
        virtual ~Vertex_buffer() {}
        /**
         * Binds this buffer. This function is directly taken from the OpenGL way of doing things.
         */
        virtual void bind() = 0;
        /**
         * Unbind this buffer. The opposite of binding.
         */
        virtual void unbind() = 0;
        /**
         * Set the buffer layout, simple as that.
         * @param layout
         */
        virtual void set_layout(const Buffer_layout& layout) = 0;
        virtual Buffer_layout& get_layout() = 0;
        virtual int get_instance_divisor() = 0;
        virtual void set_instance_divisor(int divisor) = 0;
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

