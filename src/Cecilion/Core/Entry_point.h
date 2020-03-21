#pragma once

#include <iostream>
#include "Event_system.h"
//#include "Log.h"

//namespace Cecilion {
//    class Application;
//
//    Application* CreateApplication();
//}
//
//extern Cecilion::Application *Cecilion::CreateApplication();

int main(int argc, char **argv) {
    Cecilion::Log::Init();
    CORE_LOG_INFO("Initialized log!");
    Cecilion::Event_system::Init();
    CORE_LOG_INFO("Entry point says hello!");

    auto app = Cecilion::CreateApplication();
    app->run();

    delete app;
//std::cout << "Hello world!" << std::endl;
}
