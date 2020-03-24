#include "Window.h"

#ifdef CECILION_PLATFORM_WINDOWS
#include <Platform/Windows/W_window.h>

#endif


namespace Cecilion {
    Window *Window::create_window(const Window_properties &properties) {
    #ifdef CECILION_PLATFORM_WINDOWS
        return new W_window(properties);
    #else
        return nullptr;
    #endif
    }
}

