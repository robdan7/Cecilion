#include <iostream>
#include <utility>
#include <bitset>
#include <vector>
#include <Utils/Sparse_set.h>

#define API_OPENGL
#include <Cecilion.h>

std::shared_ptr<Cecilion::Shader> create_shader() {
    auto vert = Cecilion::Shader::create_shader_stage(OPENGL_VERTEX_SHADER, R"(
            #version 450 core
            in vec4 in_position;
            in mat4 matrix;
            in vec4 instance_color;

            out vec4 vertex_color;
//            uniform Another_Uniform_block {
//                float scale;
//                vec4 color;
//                vec3 position;
//                mat4 matrix;
//            } another_block;

            uniform Uniform_block {
                float scale;
            } my_block;



            void main() {
                vertex_color = instance_color;
                vec4 position = in_position;
                position.x += my_block.scale;
                gl_Position =matrix* position;
            }
            )");

    auto frag = Cecilion::Shader::create_shader_stage(OPENGL_FRAGMENT_SHADER, R"(
            #version 450 core
            in vec4 vertex_color;
            out vec4 color;

            void main() {
                color = vertex_color;
            }
            )");

    auto program =  Cecilion::Shader::create_shader({vert, frag});
    program->compile();

    // TODO This should be done automatically.
    /// Bind uniform block index to block binding 0.
    GLuint index= glGetUniformBlockIndex(program->get_ID(), "Uniform_block");
    glUniformBlockBinding(program->get_ID(), index,0);

    return program;
}

std::shared_ptr<Cecilion::Shader> create_shader(const char* name) {
    std::ifstream inputStream(name, std::ios::binary);
    std::istreambuf_iterator<char> start_it(inputStream), end_it;
    std::vector<char> buffer(start_it, end_it);
    inputStream.close();
    return Cecilion::Shader::load_binary_shader(36385, buffer.data(), buffer.size());
}

std::shared_ptr<Cecilion::Vertex_buffer> create_buffer() {
    float vertices[4 * (4)] = {
            -0.1f, -0.1f, 0.0f,1, //0, 0.3, 1, 1.0,
            0.0f, -0.1f, 0.0f,1,  //0.9, 1, 0.2, 1.0,
            0.0f, 0.0f, 0.0f,1,   //0.9, 0.3, 0.2, 1.0,
            -0.1f, 0.0f, 0.0f,1//,  0.9, 0.3, 0.2, 1.0
    };
    std::shared_ptr<Cecilion::Vertex_buffer> vertex_buffer =
            Cecilion::Vertex_buffer::Create(
                    vertices,
                    sizeof(vertices),
                    Cecilion::Vertex_buffer::Access_frequency::STATIC,
                    Cecilion::Vertex_buffer::Access_type::DRAW);

    Cecilion::Buffer_layout layout = {
            {Cecilion::Shader_data::Float4, "position"}
//            {Cecilion::Shader_data::Float4, "color"}
    };
    vertex_buffer->set_layout(layout);
    return vertex_buffer;
}

std::shared_ptr<Cecilion::Index_buffer> create_index_buffer() {
    uint32_t indices[6] = {0,2,3,0,1,2};
    std::shared_ptr<Cecilion::Index_buffer> index_buffer = std::shared_ptr<Cecilion::Index_buffer>(Cecilion::Index_buffer::Create(indices, 6));
    return index_buffer;
}

/// Test layer for instanced rendering.
class Mesh_layer : public Cecilion::Layer<> {
public:
    ~Mesh_layer() override {
        CECILION_PROFILE_END_SESSION;
    }

    Mesh_layer(): filter(Cecilion::ECS::get_filter<>()){
        CECILION_PROFILE_BEGIN_SESSION("frustum culling", "frustum culling.json");

        this->m_mesh = std::shared_ptr<Cecilion::Instanced_mesh>(new Cecilion::Instanced_mesh({
            {Cecilion::Shader_data::Mat4, "matrix"},
            {Cecilion::Shader_data::Float4, "color"}}));
        this->m_mesh->add_LOD(create_buffer(), create_index_buffer(), create_shader());

        int i = 0;
        float size = 2;
        for (float x = -size; x < size; x += 0.15f) {
            for (float y = -size; y < size; y += 0.15f) {

                float matrices[16+4] = {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        x, y, 0,1,
                        0.0f,abs(x),abs(y),1
                };
                this->m_mesh->add_instance(matrices, glm::vec3{x, y, 0}, 1);
                i++;
            }
        }
        CORE_LOG_INFO("Computed {0} instances", i);
    }
    void on_update() override {
        Cecilion::Layer<>::on_update();
        this->m_mesh->on_update();
        Cecilion::Render_command::set_clear_color({0.2f,0.2f,0.2f,1.0f});
        Cecilion::Render_command::clear();
        Cecilion::Renderer::begin_scene();
        this->m_mesh->on_render();
        Cecilion::Renderer::end_scene();
    }

private:
    std::shared_ptr<Cecilion::Instanced_mesh> m_mesh;
    Cecilion::Filter<> filter;
};

/// Test code for global shader parameters.
/// ------ Shader param data ------
struct Base_container;

struct I_data : public Cecilion::Buffer_element {
private:
    uint32_t offset;
public:
    virtual void* get_data() = 0;
    [[nodiscard]] uint32_t get_offset() const {return offset;}
    virtual uint32_t stride() = 0;
    uint32_t get_offset() {return this->offset;}
    I_data(I_data&& other)  noexcept : offset(other.offset), Cecilion::Buffer_element(other) {}
    I_data(uint32_t offset, Cecilion::Shader_data type, const char* name, bool normalized = false) : Cecilion::Buffer_element(type,name,normalized),offset(offset) {}
};

struct Float_data : public I_data{
    float my_float = 0;
    Float_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Float, name, normalized){}
    Float_data(Float_data &&other) : I_data(static_cast<I_data&&>(other)) {
        this->my_float = other.my_float;
    }

    void* get_data() override {
        return &this->my_float;
    }
    uint32_t stride() {
        return sizeof(my_float);
    }
};

struct Int_data : public I_data {
    int my_data = 0;
    Int_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Int, name, normalized){}
    Int_data(Int_data &&other) : I_data(static_cast<I_data&&>(other)) {
        this->my_data = other.my_data;
    }

    void *get_data() override {
        return &this->my_data;
    }
    uint32_t stride() {return this->my_data;}
};

struct Matrix_data : public I_data {
    glm::mat4x4 my_matrix = glm::mat4x4(1.0f);
    Matrix_data(uint32_t offset, const char* name, bool normalized = false) : I_data(offset, Cecilion::Shader_data::Mat4, name, normalized){}

    Matrix_data(Matrix_data &&other) : I_data(static_cast<I_data&&>(other)) {
        this->my_matrix = other.my_matrix;
    }

    void *get_data() override {
        return &this->my_matrix[0][0];
    }
    uint32_t stride() {return sizeof(my_matrix);}
};


/// ------- Base class for shader params ------

struct Base_container {
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
    [[nodiscard]] uint32_t get_size() const {return this->size;}
    Base_container() = default;
};
#define BEGIN_SHADER_PARAMS(Name) struct Name : public Base_container { std::shared_ptr<Cecilion::Shader_param_buffer> m_buffer;\
Name() : Base_container() {this->m_buffer = Cecilion::Shader_param_buffer::Create(nullptr, this->get_size(), Cecilion::Vertex_buffer::Access_frequency::DYNAMIC, Cecilion::Vertex_buffer::Access_type::DRAW);} \
void write(I_data* data) { \
    this->m_buffer->set_sub_data((float*)data->get_data(), data->get_offset(),data->stride());\
}
#define SET_SHADER_PARAM(type, name) type name = this->set_arg<type>(#name);
#define END_SHADER_PARAMS(Name) };

/// ------ User defined shader params -------

BEGIN_SHADER_PARAMS(Data_container)
    SET_SHADER_PARAM(Float_data, scale)
END_SHADER_PARAMS(Data_container)

/// ---------------------------------------

class App : public Cecilion::Application {
public :
    App() {

        Data_container c = Data_container();
        c.scale.my_float = 1;
        c.write((I_data*)&c.scale);


//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Example_layer<>())));
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Mesh_layer())));
//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Actor_layer())));
        Cecilion::Layer<>* imgui = new Cecilion::ImGui_layer();
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(imgui)));
    }

    ~App() override {

    }

};

Cecilion::Application* Cecilion::CreateApplication() {
    return new App();
}