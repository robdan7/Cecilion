#pragma once
#include <memory>

#include "Window.h"

namespace Cecilion {
    class Application {
    public:
        Application();
        virtual ~Application();
        void run();
        static void run2();
    private:
        Window* m_window;
        bool m_running = true;
    };

    // To be defined in client
    Application* CreateApplication();
}
