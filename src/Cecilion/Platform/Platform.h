#pragma once

#include <Core/Log.h>
#include <Platform/OpenGL/GL_context.h>
#include "Renderer/Renderer_API.h"

namespace Cecilion {
/// The following functions creates the graphics API
    GLFW_context* create_engine_context() {
        #ifdef API_OPENGL   // Must be defined by the user.
            return new Cecilion::GL_context();
            #else
                CORE_ASSERT(false, "Could not find a valid API context!")
                return nullptr;
        #endif
    }

    Render::Renderer_API::API get_API(){
        #ifdef API_OPENGL   // Must be defined by the user.
            return Cecilion::Render::Renderer_API::API::OpenGL;
        #else
            CORE_ASSERT(false, "Could not find a valid API context!")
            return Cecilion::Renderer_API::API::None;
        #endif
    }
}
