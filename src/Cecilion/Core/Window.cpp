#include "Window.h"
#include "Core.h"

#if  defined(CECILION_PLATFORM_WINDOWS_X84) || defined(CECILION_PLATFORM_WINDOWS_X64)
    #include <Platform/Windows/W_window.h>
#endif


namespace Cecilion {
    Window *Window::create_window(Window_properties properties) {
        /// TODO remove separate argument and force the user to define properties.
    #if  defined(CECILION_PLATFORM_WINDOWS_X84) || defined(CECILION_PLATFORM_WINDOWS_X64)
        return new W_window(properties, Cecilion::create_engine_context());
    #else
        CORE_ASSERT(false, "The platform ain't windows, u dumdum!")
        return nullptr;
    #endif
    }
}

