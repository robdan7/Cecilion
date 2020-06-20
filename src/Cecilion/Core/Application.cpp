//#include <iostream>
#include <iostream>
#include "Application.h"

#include "Log.h"
//#include <Event/Async_inbox.h>
#include <Renderer/Render_command.h>
#include <Renderer/Renderer.h>
//#include <Renderer/Buffer.h>
#include <Event/Event_system.h>
#include "Input.h"
#include <Debug/Instrumentor.h>

namespace Cecilion {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        this->init_subscriptions();
        this->set_callback<Cecilion::Window_close_event>([this](auto event) {
            this->m_running = false;
        });
        this->append_group();   /// Set a default layer group.

        CORE_ASSERT(!this->s_instance, "Application is a lready initialized");
        s_instance = this;


        this->m_window = Window::create_window();
        CORE_LOG_INFO("Application created!");
    }

    Application::~Application() {
        delete this->m_window;
    }
    void Application::run() {

        while (this->m_running) {
            CECILION_PROFILE_SCOPE("New frame");
            {


                Event_actor_st<Window_close_event>::on_update();
                {
                    CECILION_PROFILE_SCOPE("Layer updates");
                    Layer_stack::on_update();
                }
//            for (int i = 0; i < 100; i ++) {
//                Event_system::post<Cecilion::Mouse_button_Event>(0,0);
//            }
//            this->application_layers->on_update();

////            this->m_imgui_layer->begin();
//            this->application_layers->on_imgui_render();
////            this->m_imgui_layer->end();
                this->m_window->on_update();
            }
        }
    }

    void Application::shutdown() {
        this->m_running = false;
    }
}
