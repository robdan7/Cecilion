#pragma once

#include "Application.h"

extern Cecilion::Application* Cecilion::CreateApplication();

int main(int argc, char** argv) {
    Cecilion::Log::Init();
    Cecilion::Log::GetClientLogger()->warn("Initialized Log!");
    Cecilion::Log::GetCoreLogger()->info("Core says hello!");
    auto app = Cecilion::CreateApplication();
    app->run();
    delete app;
}