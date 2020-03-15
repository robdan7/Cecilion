#pragma once

namespace Cecilion {
    class Application {
    public:
        Application();
        virtual ~Application();
        void run();
        static void run2();
    };

    // To be defined in client
    Application* CreateApplication();
}
