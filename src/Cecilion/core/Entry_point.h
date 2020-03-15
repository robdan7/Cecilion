#pragma once

#include "Application.h"

extern Cecilion::Application* Cecilion::CreateApplication();

int main(int argc, char** argv) {
    auto app = Cecilion::CreateApplication();
    app->run();
    delete app;
}