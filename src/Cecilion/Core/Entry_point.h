#pragma once

#include <iostream>
#include "Event/Event_system.h"

int main(int argc, char **argv) {
    Cecilion::Log::Init();
    CORE_LOG_INFO("Initialized log!");
    Cecilion::Event_system::Init();
    CORE_LOG_INFO("Entry point says hello!");

    auto app = Cecilion::CreateApplication();
    app->run();

    delete app;
}
