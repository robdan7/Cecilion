#include "Window.h"

#ifdef CECILION_PLATFORM_WINDOWS
#include <Platform/Windows/W_window.h>
#include "Log.h"
#include <Platform/OpenGL/GL_context.h>

#endif


namespace Cecilion {
    Window *Window::create_window(Window_properties properties) {
        // TODO remove separate argument and force the user to define properties.
        //CORE_ASSERT(context || properties.context, "Rendering context must be defined for window creation!");
//        if (!properties.context) {  // Set the context separately if we want default window values (not recommended)
//            properties.context = context;
//        }

    #ifdef CECILION_PLATFORM_WINDOWS
        #ifdef RENDER_CONTEXT_GL
            return new W_window(properties, new GL_context());
        #else
        CORE_ASSERT(context || properties.context, "Rendering context must be defined for window creation!");
        #endif
        return nullptr;
    #else
        CORE_ASSERT(false, "The platform ain't windows, u dumdum!")
        return nullptr;
    #endif
    }
}

