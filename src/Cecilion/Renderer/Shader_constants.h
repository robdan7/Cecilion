#pragma once
#include <cstdint>
#include <utility>
#include <Renderer/Buffer.h>
#include <iostream>
#include <iostream>

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

    template<typename Data>
    struct I_data : public Cecilion::Buffer_element {
        friend Data_governor<Data>;
//        friend class I_shader_constant_container;
    public:
        uint32_t offset;
        Data m_data;
    public:
        const Data* get_data() {return &this->m_data;}
        [[nodiscard]] uint32_t get_offset() const {return offset;}
        virtual uint32_t stride() {return sizeof(this->m_data);}
        const uint32_t get_offset() {return this->offset;}
    protected:
        I_data(I_data<Data>&& other)  noexcept : offset(other.offset), Cecilion::Buffer_element(other) {
            this->m_data = other.m_data;
        }
        I_data(uint32_t offset, Cecilion::Shader_data type, const char* name, bool normalized = false) : Cecilion::Buffer_element(type,name,normalized),offset(offset) {}

    };

    struct Float_data : public I_data<float>{
//        friend Data_governor<Float_data>;
        Float_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
//        Float_data(Float_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->my_float = other.my_float;
//        }
//        const float* get_data() {
//            return &this->my_float;
//        }

//        uint32_t stride() override {
//            return sizeof(my_float);
//        }
//    private:
//        float my_float = 0;
    };

    struct Float2_data : public I_data<glm::vec2>{
//        friend Data_governor<Float2_data>;
        Float2_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
//        Float2_data(Float2_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->m_vec2 = other.m_vec2;
//        }
//        const glm::vec2* get_data() {
//            return &this->m_vec2;
//        }

//        uint32_t stride() override {
//            return sizeof(m_vec2);
//        }
//    private:
//        glm::vec2 m_vec2 = glm::vec2();
    };

    struct Float3_data : public I_data<glm::vec3>{
//        friend Data_governor<Float3_data>;
        Float3_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
//        Float3_data(Float3_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->m_vec3 = other.m_vec3;
//        }
//        const glm::vec3* get_data() {
//            return &this->m_vec3;
//        }

//        uint32_t stride() override {
//            return sizeof(m_vec3);
//        }
//    private:
//        glm::vec3 m_vec3 = glm::vec3();
    };

    struct Float4_data : public I_data<glm::vec4>{
//        friend Data_governor<Float4_data>;
        Float4_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
//        Float4_data(Float4_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->m_vec3 = other.m_vec3;
//        }
//        const glm::vec4* get_data() {
//            return &this->m_vec3;
//        }

//        uint32_t stride() override {
//            return sizeof(m_vec3);
//        }
//    private:
//        glm::vec4 m_vec3 = glm::vec4();
    };

    struct Int_data : public I_data<int> {
//        friend Data_governor<Int_data>;
        Int_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
//        Int_data(Int_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->m_int = other.m_int;
//        }
//        const int *get_data()  {
//            return &this->m_int;
//        }

//        uint32_t stride() override  {return sizeof(this->m_int);}
//    private:
//        int m_int = 0;
    };

    struct Int2_data : public I_data<glm::ivec2> {
//        friend Data_governor<Int2_data>;
        Int2_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
//        Int2_data(Int2_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->m_int2 = other.m_int2;
//        }
//        const glm::ivec2 *get_data()  {
//            return &this->m_int2;
//        }

//        uint32_t stride() override {return sizeof(this->m_int2);}
//    private:
//        glm::ivec2 m_int2 = glm::ivec2();
    };

    struct Int3_data : public I_data<glm::ivec3> {
//        friend Data_governor<Int3_data>;
        Int3_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
//        Int3_data(Int3_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->m_int2 = other.m_int2;
//        }
//        const glm::ivec3 *get_data() {
//            return &this->m_int2;
//        }

//        uint32_t stride() override {return sizeof(this->m_int2);}
//    private:
//        glm::ivec3 m_int2 = glm::ivec3();
    };

    struct Int4_data : public I_data<glm::ivec4> {
//        friend Data_governor<Int4_data>;
        Int4_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
//        Int4_data(Int4_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->m_int3 = other.m_int3;
//        }
//        const glm::ivec4 *get_data() override {
//            return &this->m_int3;
//        }

//        uint32_t stride() override {return sizeof(this->m_int3);}
//    private:
//        glm::ivec4 m_int3 = glm::ivec4();
    };


    struct Matrix4_data : public I_data<glm::mat4x4> {
//        friend Data_governor<Matrix4_data>;
        Matrix4_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Mat4, name, normalized){}
//        Matrix4_data(Matrix4_data &&other) : I_data(static_cast<I_data&&>(other)) {
//            this->my_matrix = other.my_matrix;
//        }

//        const glm::mat4x4 *get_data() {
//            return &this->my_matrix;
//        }

//        uint32_t stride() override {return sizeof(my_matrix);}
//    private:
//        glm::mat4x4 my_matrix = glm::mat4x4(1.0f);
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
        virtual void write(I_data<void*>* data) = 0;
        [[nodiscard]] uint32_t get_size() const {return this->size;}
        I_shader_constant_container() = default;
    };

#define BEGIN_SHADER_CONST(Name, Governor) struct Name : public I_shader_constant_container {  friend Governor;\
Name() : I_shader_constant_container() {this->m_buffer = Cecilion::Constant_buffer::Create(nullptr, this->get_size(), Cecilion::Vertex_buffer::Access_frequency::DYNAMIC, Cecilion::Vertex_buffer::Access_type::DRAW);} \
public: void write(I_data<void*>* data) override  { \
    this->m_buffer->set_sub_data((float*)data->get_data(), data->get_offset(),data->stride());\
} \
std::shared_ptr<Cecilion::Constant_buffer> m_buffer;\
public:
#define SET_SHADER_CONST(type, name) type name = this->set_arg<type>(#name);
#define END_SHADER_CONST(Name) };


/// How to create a group of shader constants
//    BEGIN_SHADER_CONST(Data_container)
//        SET_SHADER_CONST(Float_data, scale)
//    END_SHADER_CONST(Data_container)
    class System_shader_constants;
    BEGIN_SHADER_CONST(CL_System_params, System_shader_constants)
        SET_SHADER_CONST(Matrix4_data, CL_View_matrix)             /// world view matrix
        SET_SHADER_CONST(Matrix4_data, CL_Projection_matrix)  /// view and projection matrix
        SET_SHADER_CONST(Float3_data, CL_Center_reference)      /// main camera position
        SET_SHADER_CONST(Float_data, CL_Time)
    END_SHADER_CONST(CL_System_params)

    void init_shader_constants();

    CL_System_params* get_system_params();

    /**
     * Governor class for CL_System_params
     */
//    class System_shader_constants {
//        static void test() {
//
////            params.write(nullptr);
//        }
//
//        static void request_param_lock() {
//        }
//    };
//
//
//    /**
//     * Test for float data governor.
//     */
//    class Float_governor : public Data_governor<Float_data> {
//        Float_governor(Float_data* data) : Data_governor<Float_data>(data){
//        }
//    };
//
//    class Hello {
//        void test() {
//            glm::mat4x4* data = (glm::mat4x4*)params.CL_Viewport.get_data();
//
//
//        }
//    };


}

