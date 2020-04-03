//#include <iostream>
#include <iostream>
#include "Application.h"
#include "Log.h"
#include <Event/Async_inbox.h>
#include <Platform/OpenGL/OpenGL.h>
#include <Renderer/Buffer.h>
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
            layout(location = 0) in vec3 position;


            void main() {
                gl_Position = vec4(position, 1);
            }
            )");
        uint32_t frag = GL_shader::create_shader(GL_FRAGMENT_SHADER, R"(
            #version 450 core
            layout(location = 0) out vec4 color;


            void main() {
                color =vec4(0.8, 0.3, 0.3, 1.0);
            }
            )");
        this->m_shader = std::make_unique<GL_shader>();
        m_shader->attach_shader(vert);
        m_shader->attach_shader(frag);
        m_shader->link();

        glGenVertexArrays(1, &this->m_vertex_array);
        glBindVertexArray(this->m_vertex_array);

        float vertices[3*3] = {
                -0.5f,-0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f,0.0f
        };
        this->m_vertex_buffer = Vertex_buffer::Create(vertices, sizeof(vertices));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3* sizeof(float), nullptr);

        uint32_t indices[3] = {0,1,2};
        this->m_index_buffer = Index_buffer::Create(indices, 3);

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
            glBindVertexArray(this->m_vertex_array);
            glDrawElements(GL_TRIANGLES, this->m_index_buffer->get_count(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
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
