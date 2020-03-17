#ifndef CECILION_CORE_APPLICATION_H
#define CECILION_CORE_APPLICATION_H
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
#endif
