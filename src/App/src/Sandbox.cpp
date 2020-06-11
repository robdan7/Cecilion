#include <iostream>
#include <utility>
#include <Cecilion.h>
#include <bitset>
#include <vector>
#include <typeinfo>
#include <Utils/Sparse_set.h>
#include <functional>

struct Render_object {
    Cecilion::GL_shader::Shader_program shader;
    std::shared_ptr<Cecilion::Vertex_array> m_vertex_array;
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
            arg.shader.bind();
//            Cecilion::Renderer::submit(arg.m_vertex_array,1);
            Cecilion::Renderer::submit_instanced(arg.m_vertex_array,2);
            arg.shader.unbind();
        });
        Cecilion::Renderer::end_scene();
    }

private:
    Cecilion::Filter<Render_object> filter;
};

/**
 * This creates two squares. Nothing fancy here.
 */
void create_squares() {
    auto vert = Cecilion::GL_shader::create_shader(OPENGL_VERTEX_SHADER, R"(
            #version 450 core
            layout(location = 0) in vec3 in_position;
            layout(location = 1) in vec4 in_color;
            layout(location = 2) in mat4 in_matrix;
            layout(location = 6) in vec4 instance_color;

            out vec3 out_position;
            out vec4 frag_color;



            void main() {
                frag_color = instance_color;
                out_position = in_position;
                gl_Position = in_matrix * vec4(out_position, 1);
            }
            )");
    auto frag = Cecilion::GL_shader::create_shader(OPENGL_FRAGMENT_SHADER, R"(
            #version 450 core
            in vec4 frag_color;
            layout(location = 0) out vec4 color;

            void main() {
                color =vec4(0.8, 0.3, 0.3, 1.0);
                color = frag_color;
            }
            )");

    auto shader = Cecilion::GL_shader::create_shader_program(frag, vert);

    Cecilion::GL_shader::delete_shader(vert);
    Cecilion::GL_shader::delete_shader(frag);

    float vertices[4 * (3 + 4)] = {
            -0.5f, -0.5f, 0.0f, 0.8, 1, 1, 1.0,
            0.0f, -0.5f, 0.0f, 0.3, 1, 1, 1.0,
            0.0f, 0.0f, 0.0f, 0.3, 0.3, 0.8, 1.0,
            -0.5f, 0.0f, 0.0f, 0.3, 0.3, 0.8, 1.0
    };
    std::shared_ptr<Cecilion::Vertex_buffer> vertex_buffer;
    vertex_buffer.reset(Cecilion::Vertex_buffer::Create(vertices, sizeof(vertices)));

    Cecilion::Buffer_layout layout = {
            {Cecilion::Shader_data::Float3, "position"},
            {Cecilion::Shader_data::Float4, "color"}
    };
    vertex_buffer->set_layout(layout);

    /// Remember that matrices in OpenGL are column-major, not row major.
    float matrices [16*2] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0.75f,0,0,1,

            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1,
    };
    auto instance_buffer = Cecilion::Vertex_buffer::Create(matrices, sizeof(matrices));
    instance_buffer->set_layout(Cecilion::Buffer_layout{
            {Cecilion::Shader_data::Mat4, "matrix"}
    });
    instance_buffer->set_instance_divisor(1);


    float colors[4*2] = {
            1,0,0,1,
            0,1,0,1,
    };
    auto instance_color = Cecilion::Vertex_buffer::Create(colors, sizeof(colors));
    instance_color->set_layout(Cecilion::Buffer_layout{{Cecilion::Shader_data::Float4, "Instance_color"}});
    instance_color->set_instance_divisor(1);


    std::shared_ptr<Cecilion::Vertex_array> m_vertex_array;
    m_vertex_array.reset(Cecilion::Vertex_array::Create());
    m_vertex_array->add_vertex_buffer(vertex_buffer);
    m_vertex_array->add_vertex_buffer(std::shared_ptr<Cecilion::Vertex_buffer>(instance_buffer));
    m_vertex_array->add_vertex_buffer(std::shared_ptr<Cecilion::Vertex_buffer>(instance_color));

    uint32_t indices[6] = {0,2,3,0,1,2};
    std::shared_ptr<Cecilion::Index_buffer> index_buffer;
    index_buffer.reset(Cecilion::Index_buffer::Create(indices, 6));
    m_vertex_array->set_index_buffer(index_buffer);

    auto entity = Cecilion::ECS::create_entity();
    Cecilion::ECS::add_component<Render_object>(entity, shader, m_vertex_array);
}

class App : public Cecilion::Application {
public :
    App() {
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Example_layer<>())));
        Cecilion::Layer<>* imgui = new Cecilion::ImGui_layer();
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(imgui)));
        create_squares();
    }

    ~App() {

    }

};



struct Position {
    float x,y,z;
};

/**
 * Everything that exists visually or physically within the game world has an actor, ie a position.
 */
struct Actor {
    Position m_position;
};

/**
 * The primitive mesh is the thing that is loaded from memory. Its just a dummy mesh
 * with a vertex buffer full of data. This buffer should contain all parts of a multi
 * material mesh.
 */
//struct Primitive_mesh {
//private:
//    struct Mesh_info {
//    private:
//        int start_index, size;
//    public:
//        Mesh_info(int start_index, int size) : start_index(start_index), size(size){}
//    };
//
//    std::unique_ptr<Cecilion::Vertex_buffer> my_buffer;
//    std::vector<Mesh_info> m_mesh_parts;
//    char* source;
//    int ref_counter;
//public:
//    Primitive_mesh(char* source) : source(source), ref_counter(0) {
//        float vertices[4 * (3 + 4)] = {
//                0.1f, 0.1f, 0.0f, 0.8, 0.3, 0.3, 1.0,
//                0.5f, 0.1f, 0.0f, 0.3, 0.8, 0.3, 1.0,
//                .5f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0,
//                0.1f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0
//        };
//        this->my_buffer.reset(Cecilion::Vertex_buffer::Create(vertices, sizeof(vertices)));
//        this->m_mesh_parts.push_back(Mesh_info{0, sizeof(vertices)});
//    }
//    void dereference() {this->ref_counter--;}
//    void reference() {this->ref_counter++;}
//};
//
//struct Material {
//    char* name;
//};
//
///**
// * this is one primitive mesh with one or more materials
// * depending on how many mesh parts there are.
// */
//struct Static_mesh_LOD {
//    std::shared_ptr<Primitive_mesh> p_meshes;
//    std::vector<Material> m_materials;
//};
//
//struct Static_mesh_source {
//    std::unique_ptr<Static_mesh_LOD> LODs;
//};

Cecilion::Application* Cecilion::CreateApplication() {
    LOG_INFO("Sandbox says hello!");
    return new App();
}