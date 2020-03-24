//#include <iostream>
#include <iostream>
#include "Application.h"
#include "Log.h"
// TODO App layers for input handling, rendering and other stuff, y'now.
namespace Cecilion {
    void Application::window_close_callback(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        CORE_LOG_INFO("User closed the window. Shutting down Cecilion.");
        dynamic_cast<Application*>(actor.get())->shutdown();
    }




    Application::Application() : I_Event_actor("Cecilion_application") {
        this->m_window = Window::create_window();
        this->subscribe_to(WINDOW_CLOSE_EVENT, &Cecilion::Application::window_close_callback);
    }
    Application::~Application() {
        delete this->m_window;
    }
    void Application::run() {

        while (this->m_running) {

            //std::cout << "Hello world from applicationn"<<std::endl;
            this->m_window->on_update();
        }
    }

    void Application::shutdown() {
        this->m_running = false;
    }
}
