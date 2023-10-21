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

#include "GameNode.h"
#include "Scene.h"

/**
 * These functions declarations are used to configure
 * a proper rendering API. The definition can be found in Platform.h
 */
namespace Cecilion {
}







