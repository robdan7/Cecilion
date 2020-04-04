//#include <iostream>
#include <iostream>
#include "Application.h"
#include "Log.h"
#include <Event/Async_inbox.h>
#include <Platform/OpenGL/OpenGL.h>
//#include <Renderer/Buffer.h>
#include "Input.h"

// TODO App layers for input handling, rendering and other stuff, y'now.
namespace Cecilion {
    Application* Application::s_instance = nullptr;

    void Application::window_close_callback(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        CORE_LOG_INFO("User closed the window. Shutting down Cecilion.");
        dynamic_cast<Application*>(actor.get())->shutdown();
    }


    Application::Application() : I_Event_actor("Cecilion_application") {
        CORE_ASSERT(!this->s_instance, "Application is a lready initialized");
        s_instance = this;

        this->actor_inbox = std::make_shared<Async_inbox>(std::shared_ptr<I_Event_actor>(this));
        this->m_window = Window::create_window();
        this->subscribe_to(WINDOW_CLOSE_EVENT, &Cecilion::Application::window_close_callback);
        this->application_layers = new Layer_stack();
        this->m_imgui_layer = std::make_shared<ImGui_layer>();
        this->push_overlay(this->m_imgui_layer);

        uint32_t vert = GL_shader::create_shader(GL_VERTEX_SHADER, R"(
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
        uint32_t frag = GL_shader::create_shader(GL_FRAGMENT_SHADER, R"(
            #version 450 core
            in vec4 frag_color;
            layout(location = 0) out vec4 color;

            void main() {
                color =vec4(0.8, 0.3, 0.3, 1.0);
                color = frag_color;
            }
            )");
        this->m_shader = std::make_unique<GL_shader>();
        m_shader->attach_shader(vert);
        m_shader->attach_shader(frag);
        m_shader->link();
//
////        glGenVertexArrays(1, &this->m_vertex_array);
////        glBindVertexArray(this->m_vertex_array);



        float vertices[4 * (3 + 4)] = {
                -0.5f, -0.5f, 0.0f, 0.8, 0.3, 0.3, 1.0,
                0.5f, -0.5f, 0.0f, 0.3, 0.8, 0.3, 1.0,
                0.5f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0,
                -0.5f, 0.5f, 0.0f, 0.3, 0.3, 0.8, 1.0
        };
        std::shared_ptr<Vertex_buffer> vertex_buffer;
        vertex_buffer.reset(Vertex_buffer::Create(vertices, sizeof(vertices)));
        {
            Cecilion::Buffer_layout layout = {
                    {Shader_data::Float3, "position"},
                    {Shader_data::Float4, "color"}
            };
            vertex_buffer->set_layout(layout);
        }

        this->m_vertex_array.reset(Vertex_array::Create());
        this->m_vertex_array->add_vertex_buffer(vertex_buffer);
//
//
        uint32_t indices[6] = {0,1,2,0,2,3};
        std::shared_ptr<Index_buffer> index_buffer;
        index_buffer.reset(Index_buffer::Create(indices, 6));
        this->m_vertex_array->set_index_buffer(index_buffer);

        //this->m_shader.reset();
    }

    Application::~Application() {
        delete this->m_window;
    }
    void Application::run() {

        while (this->m_running) {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2,0.2,0.2,1);

            this->m_shader->bind();
//            glBegin(GL_TRIANGLES);
//            glVertex3f(-0.5,-0.5,0);
//            glVertex3f(0.5,-0.5,0);
//            glVertex3f(0,0.5,0);
//            glEnd();

            m_vertex_array->bind();
            glDrawElements(GL_TRIANGLES, this->m_vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
            m_vertex_array->unbind();

//            glBindVertexArray(this->m_vertex_array);
//            glDrawElements(GL_TRIANGLES, this->m_index_buffer->get_count(), GL_UNSIGNED_INT, nullptr);
//            glBindVertexArray(0);
            this->m_shader->unbind();

            this->application_layers->on_update();

            this->m_imgui_layer->begin();
            this->application_layers->on_imgui_render();
            this->m_imgui_layer->end();

            this->m_window->on_update();
        }
    }

    void Application::shutdown() {
        this->m_running = false;
    }

    void Application::push_layer(const std::shared_ptr<Application_layer> &layer) {
        this->application_layers->push_layer(layer, false);
    }

    void Application::pop_layer(const std::shared_ptr<Application_layer> &layer) {
        this->application_layers->pop_layer(layer, false);
    }

    void Application::push_overlay(const std::shared_ptr<Application_layer> &layer) {
        this->application_layers->push_layer(layer, true);
    }

    void Application::pop_overlay(const std::shared_ptr<Application_layer> &layer) {
        this->application_layers->pop_layer(layer, true);
    }
}
