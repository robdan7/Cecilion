#pragma once

#include <iostream>
#include <Debug/Instrumentor.h>
//#include "Event/Event_system.h"

/**
 * Pure entry point for the engine. Nothing fancy going on here.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
    Cecilion::Log::Init();
    CORE_LOG_INFO("Initialized log!");
    CORE_LOG_INFO("Entry point says hello!");

    auto app = Cecilion::CreateApplication();
    app->run();

    delete app;
}
