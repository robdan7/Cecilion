#include "Vertex_array.h"
#include "Renderer.h"
#include <Platform/OpenGL/GL_vertex_array.h>
namespace Cecilion {
    Cecilion::Vertex_array::~Vertex_array() {

    }

    Cecilion::Vertex_array *Cecilion::Vertex_array::Create() {
        switch (Renderer::get_API()) {
            case Renderer_API::API::None :
                CORE_ASSERT(false, "Renderer_API:: No render API is defined!");
                break;
            case Renderer_API::API::OpenGL:
                return new GL_vertex_array();
        }
        CORE_ASSERT(false, "Unknown Renderer_API!");
        return nullptr;
    }
}

