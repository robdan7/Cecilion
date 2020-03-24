//#include <iostream>
#include <iostream>
#include "Application.h"

namespace Cecilion {
    Application::Application() {
        this->m_window = Window::create_window();
    }
    Application::~Application() {

    }
    void Application::run() {

        while (this->m_running) {
            //std::cout << "Hello world from applicationn"<<std::endl;
            this->m_window->on_update();
        }
    }

    void Application::run2() {
        //std::cout << "This is the application in Cecilion folder."<< std::endl;
    }
}
