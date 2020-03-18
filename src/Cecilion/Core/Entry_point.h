#ifndef CECILION_CORE_ENTRY_POINT_H
#define CECILION_CORE_ENTRY_POINT_H

#include <iostream>

extern Cecilion::Application *Cecilion::CreateApplication();

int main(int argc, char **argv) {
    Cecilion::Log::Init();
    CORE_LOG_INFO("Initialized log!");
    Cecilion::EventSystem::Init();
    CORE_LOG_INFO("Entry point says hello!");

    auto app = Cecilion::CreateApplication();
    app->run();

    delete app;
}

#endif