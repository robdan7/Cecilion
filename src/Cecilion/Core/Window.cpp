#include "Window.h"
#include "Core.h"

#if  defined(CECILION_PLATFORM_WINDOWS_X84) || defined(CECILION_PLATFORM_WINDOWS_X64)
    #include <Platform/Windows/W_window.h>
#endif


namespace Cecilion {
    Window *Window::create_window(Window_properties properties) {
        /// TODO remove separate argument and force the user to define properties.
        /// this code is probably dead and is not needed.
        //CORE_ASSERT(context || properties.context, "Rendering context must be defined for window creation!");
//        if (!properties.context) {  // Set the context separately if we want default window values (not recommended)
//            properties.context = context;
//        }

    #if  defined(CECILION_PLATFORM_WINDOWS_X84) || defined(CECILION_PLATFORM_WINDOWS_X64)
        return new W_window(properties, Cecilion::create_engine_context());
    #else
        CORE_ASSERT(false, "The platform ain't windows, u dumdum!")
        return nullptr;
    #endif
    }
}

