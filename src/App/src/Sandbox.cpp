#include <iostream>
#include <utility>
#include <bitset>
#include <vector>
#include <typeinfo>
#include <Utils/Sparse_set.h>
#include <functional>

#define API_OPENGL
#include <Cecilion.h>
#include <Platform/OpenGL/GL_Frustum_compute_query.h>

#include <thread>




struct Render_object {
public:
    std::shared_ptr<Cecilion::Shader> shader;
    std::shared_ptr<Cecilion::Vertex_array> m_vertex_array;
    template<typename... Args>
    Render_object(std::shared_ptr<Cecilion::Vertex_array> array, std::shared_ptr<Cecilion::Shader> shader) : m_vertex_array(array), shader(shader) {

    }

    ~Render_object() {

    }
};

template<typename... Event>
class Example_layer : public Cecilion::Layer<Event...> {
public:
    Example_layer(): filter(Cecilion::ECS::get_filter<Render_object>()){


    }
    void on_update() override {
        Cecilion::Layer<Event...>::on_update();

        Cecilion::Render_command::set_clear_color({0.2f,0.2f,0.2f,1.0f});
        Cecilion::Render_command::clear();

        Cecilion::Renderer::begin_scene();
        this->filter.for_each_compact([](Render_object& arg){
            arg.shader->bind();
//            Cecilion::Renderer::submit(arg.m_vertex_array,1);
            Cecilion::Renderer::submit_instanced(arg.m_vertex_array,2);
            arg.shader->unbind();
        });
        Cecilion::Renderer::end_scene();
    }

private:
    Cecilion::Filter<Render_object> filter;
};


std::shared_ptr<Cecilion::Shader> create_shader() {
    auto vert = Cecilion::Shader::create_shader_stage(OPENGL_VERTEX_SHADER, R"(
            #version 450 core
            in vec4 in_position;
            in mat4 matrix;
            in vec4 instance_color;

            out vec4 vertex_color;
            uniform Another_Uniform_block {
                float scale;
                vec4 color;
                vec3 position;
                mat4 matrix;
            } another_block;

            uniform Uniform_block {
                float scale;
                vec4 color;
                vec3 position;
                mat4 matrix;
            } my_block;



            void main() {
                vertex_color = instance_color;
                gl_Position =matrix* in_position;
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


    /// Passthrough geometry shader.
//    auto geometry = Cecilion::Shader::create_shader_stage(OPENGL_GEOMETRY_SHADER, R"(
//        #version 450 core
//        layout (triangles) in;
//        layout (triangle_strip, max_vertices = 3) out;
//
//        in vec4 vert_color[];
//        in int ID[];
//        out vec4 frag_color;
//        void main() {
//            for (int i = 0; i < gl_in.length(); i++) {
//                if(mod(ID[i],2) == 0) {
//
//                    frag_color = vert_color[i];
//                } else {
//                    frag_color = vec4(0.2,0.8,0.2,1);
//                }
//                gl_Position = gl_in[i].gl_Position;
//                EmitVertex();
//
//            }
//            EndPrimitive();
//        }
//    )");
    auto program =  Cecilion::Shader::create_shader({vert, frag});
//    program->compile();
//    program->attach_shader_stage(geometry);
    program->compile();

//    GLuint index= glGetUniformBlockIndex(program->get_ID(), "Uniform_block");
//    LOG_INFO("Uniform index: {0}", index);


    return program;
}

std::shared_ptr<Cecilion::Shader> create_shader(const char* name) {
    std::ifstream inputStream(name, std::ios::binary);
////            std::ifstream input_stream(fName, std::ios::binary);
    std::istreambuf_iterator<char> start_it(inputStream), end_it;
    std::vector<char> buffer(start_it, end_it);
    inputStream.close();


//            CORE_LOG_INFO("Wrote {0} bytes to char vector. Name of shader: ", buffer.size(), name);
//    glProgramBinary(this->m_program, format, buffer.data(), buffer.size() );
    return Cecilion::Shader::load_binary_shader(36385, buffer.data(), buffer.size());
}

std::shared_ptr<Cecilion::Vertex_buffer> create_buffer() {
    float vertices[4 * (4)] = {
            -0.1f, -0.1f, 0.0f,1, //0, 0.3, 1, 1.0,
            0.0f, -0.1f, 0.0f,1,  //0.9, 1, 0.2, 1.0,
            0.0f, 0.0f, 0.0f,1,   //0.9, 0.3, 0.2, 1.0,
            -0.1f, 0.0f, 0.0f,1//,  0.9, 0.3, 0.2, 1.0
    };
    std::shared_ptr<Cecilion::Vertex_buffer> vertex_buffer;
    vertex_buffer.reset(Cecilion::Vertex_buffer::Create(vertices, sizeof(vertices), Cecilion::Vertex_buffer::Access_frequency::STATIC, Cecilion::Vertex_buffer::Access_type::DRAW));

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
        int size = 2;
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

//        this->m_mesh->on_update();
//
//        {
//            CECILION_PROFILE_SCOPE("Rendering mesh");
//            this->m_mesh->on_render();
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        this->m_mesh->on_update();
//        {
//            CECILION_PROFILE_SCOPE("Rendering mesh");
//            this->m_mesh->on_render();
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        this->m_mesh->on_update();
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        this->m_mesh->on_update();
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        this->m_mesh->on_update();

//        CORE_LOG_INFO("Culled instances: {0}", this->m_mesh->m_frustum_pipeline->fetch_result());
    }
    void on_update() override {
        CECILION_PROFILE_SCOPE("Updating mesh layer");
        Cecilion::Layer<>::on_update();

        {
//            CECILION_PROFILE_SCOPE("Updating mesh");
            this->m_mesh->on_update();
        }

        {
            CECILION_PROFILE_SCOPE("Rendering scene layer");
            Cecilion::Render_command::set_clear_color({0.2f,0.2f,0.2f,1.0f});
            Cecilion::Render_command::clear();

            Cecilion::Renderer::begin_scene();
            {
//            CECILION_PROFILE_SCOPE("sleeping for one millisecond");
//            std::this_thread::sleep_for(std::chrono::milliseconds(1));
//                CECILION_PROFILE_SCOPE("Rendering meshes");
                this->m_mesh->on_render();
            }

            Cecilion::Renderer::end_scene();
        }

//        {
//            CECILION_PROFILE_SCOPE("Doing nothing");
//            for (int i = 0; i<60000; i++) {
//
//            }
//        }
    }

private:

    std::shared_ptr<Cecilion::Instanced_mesh> m_mesh;
    Cecilion::Filter<> filter;
};
//struct Uniform_data {
//    uint32_t my_number;
//    Uniform_data(int i) {this->my_number = i;}
//};



//#define INCREASE (__COUNTER__)
//#define BEGIN_UNIFORM(Name, ...) struct Name { \
//private:\
//    uint32_t total = 0;\
//    uint32_t test() {this->data.emplace_back(total); LOG_INFO("Size: {0}", this->data.back());return this->total++;} \
//    public:\
//    Uniform_data get(uint32_t ID) {return this->data[ID];}
//#define INIT_VAR(Name) uint32_t Name = test()
//#define END_UNIFORM  std::vector<Uniform_data> data;};


//    static const int my_array[std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value]; \
};
//#define SET_VAR(Uniform, Name) const int Uniform::Name = __COUNTER__

//BEGIN_UNIFORM(My_uniform)
//    INIT_VAR(Hello);
//    INIT_VAR(world);
//    INIT_VAR(byebye);
////    Uniform_data var{INCREASE};
//END_UNIFORM



/// ------ Data ------
struct Base_container;


struct I_data {
    friend Base_container;
private:
    uint32_t offset;
public:
    virtual void* get_data() = 0;
    uint32_t get_offset() {return offset;}
    virtual uint32_t stride() = 0;
    I_data(I_data&& other) : offset(other.offset){}
    I_data(uint32_t offset) : offset(offset) {}
};

struct Float_data : public I_data{
    float my_float = 0;
    Float_data(uint32_t offset) : I_data(offset){}
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
    Int_data(uint32_t offset) : I_data(offset){}
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
    Matrix_data(uint32_t offset) : I_data(offset){}

    Matrix_data(Matrix_data &&other) : I_data(static_cast<I_data&&>(other)) {
        this->my_matrix = other.my_matrix;
    }

    void *get_data() override {
        return &this->my_matrix[0][0];
    }
    uint32_t stride() {return sizeof(my_matrix);}
};
/// ------- Base class container ------

struct Base_container {
private:
protected:

    uint32_t size = 0;
    template<typename Arg>
    Arg set_arg(Arg&& arg) {
        arg.offset= size;
        this->size += arg.stride();
        return std::move(arg);
    }
public:
//    std::shared_ptr<Cecilion::Vertex_buffer> m_buffer;
    uint32_t get_size() {return this->size;}
    Base_container() {}
//    void init_buffer() {
//        this->m_buffer = std::shared_ptr<Cecilion::Vertex_buffer>(Cecilion::Vertex_buffer::Create(nullptr, this->get_size(), Cecilion::Vertex_buffer::Access_frequency::DYNAMIC, Cecilion::Vertex_buffer::Access_type::DRAW));
//    }
};
#define BEGIN_SHADER_PARAMS(Name) struct Name : public Base_container { std::shared_ptr<Cecilion::Vertex_buffer> m_buffer;\
Name() : Base_container() {this->m_buffer = std::shared_ptr<Cecilion::Vertex_buffer>(\
Cecilion::Vertex_buffer::Create(nullptr, this->get_size(), Cecilion::Vertex_buffer::Access_frequency::DYNAMIC, Cecilion::Vertex_buffer::Access_type::DRAW));} \
void write(I_data&& data) { \
    this->m_buffer->set_sub_data((float*)data.get_data(), data.get_offset(),data.stride());\
}
#define SET_SHADER_PARAM(type, name) type name = this->set_arg<type>((type &&)name);
#define END_SHADER_PARAMS(Name) };

/// ------ User defined containers -------


BEGIN_SHADER_PARAMS(Data_container)
//    SET_GPU_VAR(Matrix_data, my_matrix)
    SET_SHADER_PARAM(Float_data, my_float)
    SET_SHADER_PARAM(Int_data, my_int)
END_SHADER_PARAMS(Data_container)

BEGIN_SHADER_PARAMS(Another_container)
    SET_SHADER_PARAM(Matrix_data, my_matrix)
//    INIT_SHADER_VAR(Int_data, my_int)
END_SHADER_PARAMS(Another_container)

/// ---------------------------------------

class App : public Cecilion::Application {
public :
    App() {
//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Example_layer<>())));
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Mesh_layer())));
//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Actor_layer())));
        Cecilion::Layer<>* imgui = new Cecilion::ImGui_layer();
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(imgui)));

//        GLint data;
//        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &data);
//        LOG_INFO("Max buffer bindings: {0}", data);

//        GLint formats;
//        glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, &formats);
//        if( formats < 1 ) {
//            std::cerr << "Driver does not support any binary formats." << std::endl;
//            exit(EXIT_FAILURE);
//        }
//        LOG_INFO("Formats: {0}", formats);


        Data_container c = Data_container();
//        c.init_buffer();
    Another_container c2;

//        LOG_INFO("Size of container 1: {0}", c.get_size());
//        LOG_INFO("Size of buffer: {0}", c.m_buffer->get_size());
//        LOG_INFO("Size of object: {0}",  c2.m_buffer->get_size());

//    LOG_INFO("Size of container 2: {0}", c2.get_size());
//    LOG_INFO("offset of float: {0}, stride of float: {1}", c.my_float.get_offset(), c.my_float.stride());
//    LOG_INFO("offset of int: {0}", c.my_int.get_offset());



    }

    ~App() {

    }

};

Cecilion::Application* Cecilion::CreateApplication() {



    return new App();
}