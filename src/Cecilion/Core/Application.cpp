#include "Application.h"
#include <Renderer/Shader_constants.h>


namespace Cecilion {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        this->set_callback<Cecilion::Window_close_event>([this](auto event) {
            this->shutdown();
        });
        this->append_group();   /// Set a default layer group.

        CORE_ASSERT(!this->s_instance, "Application is already initialized");
        s_instance = this;

        this->m_window = Window::create_window();
        CORE_LOG_INFO("Application created!");
        Cecilion::init_shader_constants();
    }

    Application::~Application() {
        delete this->m_window;
    }

    /**
     * Super simple game loop.
     */
    void Application::run() {
        while (this->m_running) {
            Event_actor_st<Window_close_event>::on_update();
            Layer_stack::on_update();
            this->m_window->on_update();
        }
    }

    void Application::shutdown() {
        this->m_running = false;
    }
}
