#include <iostream>
#include <utility>
#include <Cecilion.h>
#include <bitset>
#include <vector>
#include <typeinfo>
#include <Utils/Sparse_set.h>
#include <functional>

struct Render_object {
    Cecilion::GL_shader* m_shader;
    std::shared_ptr<Cecilion::Vertex_array> m_vertex_array;
};

template<typename... Event>
class Example_layer : public Cecilion::Layer<Event...> {
public:
    Example_layer(): filter(Cecilion::ECS::get_filter<Render_object>()){
        uint32_t vert = Cecilion::GL_shader::create_shader(OPENGL_VERTEX_SHADER, R"(
            #version 450 core
            layout(location = 0) in vec3 in_position;
            layout(location = 1) in vec4 in_color;

            out vec3 out_position;
            out vec4 frag_color;



            void main() {
                frag_color = in_color;
                out_position = in_position;
                gl_Position = vec4(out_position, 1);
            }
            )");
        uint32_t frag = Cecilion::GL_shader::create_shader(OPENGL_FRAGMENT_SHADER, R"(
            #version 450 core
            in vec4 frag_color;
            layout(location = 0) out vec4 color;

            void main() {
                color =vec4(0.8, 0.3, 0.3, 1.0);
                color = frag_color;
            }
            )");
        this->m_shader = new Cecilion::GL_shader();
        m_shader->attach_shader(vert);
        m_shader->attach_shader(frag);
        m_shader->link();
        Cecilion::GL_shader::delete_shader(vert);
        Cecilion::GL_shader::delete_shader(frag);

        float vertices[4 * (3 + 4)] = {
                -0.5f, -1.0f, 0.0f, 0.8, 0.3, 0.3, 1.0,
                0.5f, -1.0f, 0.0f, 0.3, 0.8, 0.3, 1.0,
                0.5f, 0.0f, 0.0f, 0.3, 0.3, 0.8, 1.0,
                -0.5f, 0.0f, 0.0f, 0.3, 0.3, 0.8, 1.0
        };
        std::shared_ptr<Cecilion::Vertex_buffer> vertex_buffer;
        vertex_buffer.reset(Cecilion::Vertex_buffer::Create(vertices, sizeof(vertices)));

        Cecilion::Buffer_layout layout = {
                {Cecilion::Shader_data::Float3, "position"},
                {Cecilion::Shader_data::Float4, "color"}
        };
        vertex_buffer->set_layout(layout);


        this->m_vertex_array.reset(Cecilion::Vertex_array::Create());
        this->m_vertex_array->add_vertex_buffer(vertex_buffer);

        uint32_t indices[6] = {0,1,2,0,2,3};
        std::shared_ptr<Cecilion::Index_buffer> index_buffer;
        index_buffer.reset(Cecilion::Index_buffer::Create(indices, 6));
        this->m_vertex_array->set_index_buffer(index_buffer);

        auto entity = Cecilion::ECS::create_entity();
        Cecilion::ECS::add_component<Render_object>(entity, this->m_shader, this->m_vertex_array);

        float vertices2[4 * (3 + 4)] = {
                0.1f, 0.1f, 0.0f, 0.8, 0.3, 0.3, 1.0,
                0.5f, 0.1f, 0.0f, 0.3, 0.8, 0.3, 1.0,
                .5f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0,
                0.1f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0
        };

        std::shared_ptr<Cecilion::Vertex_buffer> buffer2;
        buffer2.reset(Cecilion::Vertex_buffer::Create(vertices2, sizeof(vertices2)));

        Cecilion::Buffer_layout layout2 = {
                {Cecilion::Shader_data::Float3, "position"},
                {Cecilion::Shader_data::Float4, "color"}
        };
        buffer2->set_layout(layout2);

        std::shared_ptr<Cecilion::Vertex_array> array2;
        array2.reset(Cecilion::Vertex_array::Create());
        array2->add_vertex_buffer(buffer2);

        std::shared_ptr<Cecilion::Index_buffer> index2;
        index2.reset(Cecilion::Index_buffer::Create(indices, 6));
        array2->set_index_buffer(index2);

        auto entity2 = Cecilion::ECS::create_entity();
        Cecilion::ECS::add_component<Render_object>(entity2, this->m_shader, array2);

    }
    void on_update() override {
        Cecilion::Layer<Event...>::on_update();

        Cecilion::Render_command::set_clear_color({0.2f,0.2f,0.2f,1.0f});
        Cecilion::Render_command::clear();

        Cecilion::Renderer::begin_scene();
        this->filter.for_each_compact([](Render_object& arg){
            arg.m_shader->bind();
            Cecilion::Renderer::submit(arg.m_vertex_array);
            arg.m_shader->unbind();
        });
        Cecilion::Renderer::end_scene();
    }

private:
    Cecilion::Filter<Render_object> filter;
    Cecilion::GL_shader* m_shader;
    std::shared_ptr<Cecilion::Vertex_array> m_vertex_array;
};



class App : public Cecilion::Application {
public :
    App() {
        Cecilion::Layer<>* imgui = new Cecilion::ImGui_layer();
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(imgui)));
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Example_layer<>())));
    }

    ~App() {

    }

};


Cecilion::Application* Cecilion::CreateApplication() {
    LOG_INFO("Sandbox says hello!");
    return new App();
}