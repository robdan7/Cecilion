#ifndef CECILION_CORE_ENTRY_POINT_H
#define CECILION_CORE_ENTRY_POINT_H
#include "Application.h"

extern Cecilion::Application* Cecilion::CreateApplication();

int main(int argc, char** argv) {
    Cecilion::Log::Init();
    CORE_LOG_INFO("Initialized log!");
    LOG_INFO("App ways hello!");
    LOG_WARN("App ways hello!");
    //Cecilion::Log::GetClientLogger()->warn("Initialized Log!");
    //Cecilion::Log::GetCoreLogger()->info("Core says hello!");
    auto app = Cecilion::CreateApplication();
    app->run();
    delete app;
}

#endif