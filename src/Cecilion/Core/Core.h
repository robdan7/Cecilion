#pragma once
#define CECILION_ENABLE_ASSERTS

/// define what platform we're compiling to.
#ifdef _WIN32
#ifdef _WIN64
#define CECILION_PLATFORM_WINDOWS_X64
#else
#define CECILION_PLATFORM_WINDOWS_X84
#endif
#else
#error "Your platform is not supported!"
#endif

/**
 * These functions declarations are used to configure
 * a proper rendering API. The definition can be found in Platform.h
 */
#include <Renderer/GLFW_context.h>
#include "Renderer/Renderer_API.h"
namespace Cecilion {
    GLFW_context* create_engine_context();
    Cecilion::Render::Renderer_API::API get_API();
}







