#include "Application.h"

namespace Cecilion {

#define TEST (Cecilion::type())

    Application* Application::s_instance = nullptr;

    Application::Application() {
        this->append_group();   /// Set a default layer group.

        CORE_ASSERT(!this->s_instance, "Application is already initialized");
        s_instance = this;

        CORE_LOG_INFO("Application created!");
    }

    Application::~Application() {
    }

    /**
     * Super simple game loop.
     */
    void Application::run() {
        while (this->m_running) {
            Layer_stack::on_update();
        }
    }

    void Application::shutdown() {
        this->m_running = false;
    }
}
