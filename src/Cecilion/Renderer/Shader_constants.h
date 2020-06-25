#pragma once
#include <cstdint>
#include <utility>
#include <Renderer/Buffer.h>

/// Test code for global shader parameters.
/// ------ Shader param data ------

namespace Cecilion {
    struct I_shader_constant_container;

    template<typename Data>
    class Data_governor {
    public:
        explicit Data_governor(Data* data) : p_data(data) {}
    protected:
        Data* p_data;
    };

    struct I_data : public Cecilion::Buffer_element {
//        friend class I_shader_constant_container;
    private:
        uint32_t offset;
    public:
        virtual const void* get_data() = 0;
        [[nodiscard]] uint32_t get_offset() const {return offset;}
        virtual uint32_t stride() = 0;
        const uint32_t get_offset() {return this->offset;}
        I_data(I_data&& other)  noexcept : offset(other.offset), Cecilion::Buffer_element(other) { }
        I_data(uint32_t offset, Cecilion::Shader_data type, const char* name, bool normalized = false) : Cecilion::Buffer_element(type,name,normalized),offset(offset) {}

    };

    struct Float_data : public I_data{
        friend Data_governor<Float_data>;
        float my_float = 0;
        Float_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
        Float_data(Float_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->my_float = other.my_float;
        }

        const void* get_data() {
            return &this->my_float;
        }
        uint32_t stride() override {
            return sizeof(my_float);
        }
    };

    struct Float2_data : public I_data{
        friend Data_governor<Float2_data>;
        glm::vec2 m_vec2 = glm::vec2();
        Float2_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
        Float2_data(Float2_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->m_vec2 = other.m_vec2;
        }

        const void* get_data() override {
            return &this->m_vec2;
        }
        uint32_t stride() override {
            return sizeof(m_vec2);
        }
    };

    struct Float3_data : public I_data{
        friend Data_governor<Float3_data>;
        glm::vec3 m_vec3 = glm::vec3();
        Float3_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
        Float3_data(Float3_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->m_vec3 = other.m_vec3;
        }

        const void* get_data() override {
            return &this->m_vec3;
        }
        uint32_t stride() override {
            return sizeof(m_vec3);
        }
    };

    struct Float4_data : public I_data{
        friend Data_governor<Float4_data>;
        glm::vec4 m_vec3 = glm::vec4();
        Float4_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
        Float4_data(Float4_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->m_vec3 = other.m_vec3;
        }

        const void* get_data() override {
            return &this->m_vec3;
        }
        uint32_t stride() override {
            return sizeof(m_vec3);
        }
    };

    struct Int_data : public I_data {
        friend Data_governor<Int_data>;
        int m_int = 0;
        Int_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
        Int_data(Int_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->m_int = other.m_int;
        }

        const void *get_data() override {
            return &this->m_int;
        }
        uint32_t stride() override  {return sizeof(this->m_int);}
    };

    struct Int2_data : public I_data {
        friend Data_governor<Int2_data>;
        glm::ivec2 m_int2 = glm::ivec2();
        Int2_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
        Int2_data(Int2_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->m_int2 = other.m_int2;
        }

        const void *get_data() override {
            return &this->m_int2;
        }
        uint32_t stride() override {return sizeof(this->m_int2);}
    };

    struct Int3_data : public I_data {
        friend Data_governor<Int3_data>;
        glm::ivec3 m_int2 = glm::ivec3();
        Int3_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
        Int3_data(Int3_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->m_int2 = other.m_int2;
        }

        const void *get_data() override {
            return &this->m_int2;
        }
        uint32_t stride() override {return sizeof(this->m_int2);}
    };

    struct Int4_data : public I_data {
        friend Data_governor<Int4_data>;
        glm::ivec4 m_int3 = glm::ivec4();
        Int4_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
        Int4_data(Int4_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->m_int3 = other.m_int3;
        }

        const void *get_data() override {
            return &this->m_int3;
        }
        uint32_t stride() override {return sizeof(this->m_int3);}
    };


    struct Matrix4_data : public I_data {
        friend Data_governor<Matrix4_data>;
        glm::mat4x4 my_matrix = glm::mat4x4(1.0f);
        Matrix4_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Mat4, name, normalized){}

        Matrix4_data(Matrix4_data &&other) : I_data(static_cast<I_data&&>(other)) {
            this->my_matrix = other.my_matrix;
        }

        const void *get_data() override {
            return &this->my_matrix[0][0];
        }
        uint32_t stride() override {return sizeof(my_matrix);}
    };


/// ------- Base class for shader params ------

    struct I_shader_constant_container {
    private:
    protected:
        uint32_t size = 0;
        template<typename Arg>
        Arg set_arg(const char* name) {
            Arg a = Arg(size,name,false);
            this->size += a.stride();
            return std::move(a);
        }
    public:
        virtual void write(I_data* data) = 0;
        [[nodiscard]] uint32_t get_size() const {return this->size;}
        I_shader_constant_container() = default;
    };

#define BEGIN_SHADER_CONST(Name, Governor) struct Name : public I_shader_constant_container {  friend Governor; std::shared_ptr<Cecilion::Constant_buffer> m_buffer;\
Name() : I_shader_constant_container() {this->m_buffer = Cecilion::Constant_buffer::Create(nullptr, this->get_size(), Cecilion::Vertex_buffer::Access_frequency::DYNAMIC, Cecilion::Vertex_buffer::Access_type::DRAW);} \
private: void write(I_data* data) override  { \
    this->m_buffer->set_sub_data((float*)data->get_data(), data->get_offset(),data->stride());\
} public:
#define SET_SHADER_CONST(type, name) type name = this->set_arg<type>(#name);
#define END_SHADER_CONST(Name) };



/// How to create a group of shader constants
//    BEGIN_SHADER_CONST(Data_container)
//        SET_SHADER_CONST(Float_data, scale)
//    END_SHADER_CONST(Data_container)
    class System_shader_constants;
    BEGIN_SHADER_CONST(CL_System_params, System_shader_constants)
        SET_SHADER_CONST(Matrix4_data, CL_Viewport)
        SET_SHADER_CONST(Float_data, GL_Time)
    END_SHADER_CONST(CL_System_params)

    CL_System_params params;

    /**
     * Governor class for CL_System_params
     */
    class System_shader_constants {
        static void test() {
            params.write(nullptr);
        }

        static void request_param_lock() {
        }
    };


    /**
     * Test for float data governor.
     */
    class Float_governor : public Data_governor<Float_data> {
        Float_governor(Float_data* data) : Data_governor<Float_data>(data){
        }
    };


}

