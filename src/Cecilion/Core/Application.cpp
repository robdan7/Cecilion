//#include <iostream>
#include <iostream>
#include "Application.h"
#include "Log.h"
#include <Event/Async_inbox.h>
#include <Renderer/Render_command.h>
#include <Renderer/Renderer.h>
//#include <Renderer/Buffer.h>
#include "Input.h"

// TODO App layers for input handling, rendering and other stuff, y'now.
namespace Cecilion {
    Application* Application::s_instance = nullptr;

    Application::Application() : I_Event_actor("Cecilion_application") {
        CORE_ASSERT(!this->s_instance, "Application is a lready initialized");
        s_instance = this;

        this->actor_inbox = std::make_shared<Async_inbox>(std::shared_ptr<I_Event_actor>(this));
        this->m_window = Window::create_window();
        this->subscribe_to(typeid(Cecilion::Window_close_event), [this](Event_message* message){
            CORE_LOG_INFO("User closed the window. Shutting down Cecilion.");
            this->shutdown();
        });
        this->application_layers = new Layer_stack();
        this->m_imgui_layer = std::make_shared<ImGui_layer>();
        this->push_overlay(this->m_imgui_layer);


        //this->m_shader.reset();
    }

    Application::~Application() {
        delete this->m_window;
    }
    void Application::run() {

        while (this->m_running) {
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
