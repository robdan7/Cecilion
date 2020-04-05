#include <iostream>
#include <utility>
#include <Cecilion.h>

    struct demo: Cecilion::Event_message {
        string message;
        explicit demo(string message) : Event_message(Cecilion::SYSTEM_STARTUP_EVENT) { this->message = std::move(message);}
        ~demo() override = default;
        //[[nodiscard]] virtual demo* clone() const {return new demo(this->message);}
    };

void test(std::shared_ptr<Cecilion::I_Event_actor> actor, Cecilion::Event_message* message) {
    LOG_INFO("Pressed key!");
}

void test2(std::shared_ptr<Cecilion::I_Event_actor> actor, Cecilion::Event_message* message) {
    LOG_INFO("App pressed mouse key!");
}

class foo:public Cecilion::I_Event_actor {
public:

    void unsub() {
        this->unsubscribe(Cecilion::SYSTEM_STARTUP_EVENT);
    }

    static void call(std::shared_ptr<Cecilion::I_Event_actor> actor, Cecilion::Event_message* mess) {
        LOG_INFO(dynamic_cast<demo*>(mess)->message);
    }

    foo() : I_Event_actor("Foo") {
        this->actor_inbox = std::make_shared<Cecilion::Async_inbox>(std::shared_ptr<I_Event_actor>(this));
        this->subscribe_to(Cecilion::SYSTEM_STARTUP_EVENT, &call);
    }

    void post() {
        std::shared_ptr<Cecilion::Event_message> pointer = std::make_shared<demo>("I have sent a message to myself. Weeeee");
        Cecilion::Event_system::post(pointer);
    }
};

class Example_layer : public Cecilion::Application_layer_st {
public:
    Example_layer() {
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
        this->m_shader = std::make_unique<Cecilion::GL_shader>();
        m_shader->attach_shader(vert);
        m_shader->attach_shader(frag);
        m_shader->link();
        Cecilion::GL_shader::delete_shader(vert);
        Cecilion::GL_shader::delete_shader(frag);

        float vertices[4 * (3 + 4)] = {
                -0.5f, -0.5f, 0.0f, 0.8, 0.3, 0.3, 1.0,
                0.5f, -0.5f, 0.0f, 0.3, 0.8, 0.3, 1.0,
                0.5f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0,
                -0.5f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0
        };
        std::shared_ptr<Cecilion::Vertex_buffer> vertex_buffer;
        vertex_buffer.reset(Cecilion::Vertex_buffer::Create(vertices, sizeof(vertices)));
        {
            Cecilion::Buffer_layout layout = {
                    {Cecilion::Shader_data::Float3, "position"},
                    {Cecilion::Shader_data::Float4, "color"}
            };
            vertex_buffer->set_layout(layout);
        }

        this->m_vertex_array.reset(Cecilion::Vertex_array::Create());
        this->m_vertex_array->add_vertex_buffer(vertex_buffer);

        uint32_t indices[6] = {0,1,2,0,2,3};
        std::shared_ptr<Cecilion::Index_buffer> index_buffer;
        index_buffer.reset(Cecilion::Index_buffer::Create(indices, 6));
        this->m_vertex_array->set_index_buffer(index_buffer);
    }
    void on_update() override {
        Application_layer_st::on_update();

        Cecilion::Render_command::set_clear_color({0.2f,0.2f,0.2f,1.0f});
        Cecilion::Render_command::clear();

        Cecilion::Renderer::begin_scene();
        this->m_shader->bind();
        Cecilion::Renderer::submit(this->m_vertex_array);
        this->m_shader->unbind();
        Cecilion::Renderer::end_scene();
    }

    ~Example_layer() override {

    }
private:
    std::unique_ptr<Cecilion::GL_shader> m_shader;
    std::shared_ptr<Cecilion::Vertex_array> m_vertex_array;
};

class App : public Cecilion::Application {
public :
    App() {
        foo* f = new foo();
        for (int i = 0; i < 20; i++) {
            if (i == 10) {
                f->unsub();
            }
            f->post();
        }

        std::shared_ptr<Cecilion::Application_layer_st> layer = std::make_shared<Cecilion::Application_layer_st>();
        std::shared_ptr<Cecilion::Application_layer_st> layer2 = std::make_shared<Cecilion::Application_layer_st>();
        std::shared_ptr<Example_layer> example = std::make_shared<Example_layer>();

        this->push_layer(example);
        this->push_layer(layer);
        this->push_overlay(layer2);
//        layer2->subscribe_to(Cecilion::MOUSE_CURSOR_POS_EVENT, &test2);
        layer->subscribe_to(Cecilion::MOUSE_BUTTON_EVENT, &test2);
        layer2->subscribe_to(Cecilion::KEYBOARD_KEY_EVENT, &test);
    }

    ~App() {

    }
};


Cecilion::Application* Cecilion::CreateApplication() {
    LOG_INFO("Sandbox says hello!");
    return new App();
}