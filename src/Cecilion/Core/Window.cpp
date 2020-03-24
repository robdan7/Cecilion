#include "Window.h"

#ifdef CECILION_PLATFORM_WINDOWS
#include <Platform/Windows/W_window.h>
#include "Log.h"

#endif


namespace Cecilion {
    Window *Window::create_window(const Window_properties &properties) {
    #ifdef CECILION_PLATFORM_WINDOWS
        return new W_window(properties);
    #else
        CORE_ASSERT(false, "The platform ain't windows, u dumdum!")
        return nullptr;
    #endif
    }
}

