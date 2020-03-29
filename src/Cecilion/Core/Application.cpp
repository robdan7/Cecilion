//#include <iostream>
#include <iostream>
#include "Application.h"
#include "Log.h"
#include <Event/Async_inbox.h>
#include <Platform/OpenGL/OpenGL.h>
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
    }

    Application::~Application() {
        delete this->m_window;
    }
    void Application::run() {

        while (this->m_running) {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(1,1,0,1);
            this->application_layers->on_update();
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
