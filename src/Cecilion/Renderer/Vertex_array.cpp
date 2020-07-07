#include "Vertex_array.h"
#include "Renderer.h"
#include <Platform/OpenGL/GL_vertex_array.h>
namespace Cecilion {
    Cecilion::Vertex_array::~Vertex_array() {

    }

    std::shared_ptr<Vertex_array> Cecilion::Vertex_array::Create() {
        switch (Render::Renderer::get_API()) {
            case Render::Renderer_API::API::None :
                CORE_ASSERT(false, "Renderer_API:: No render API is defined!");
                break;
            case Render::Renderer_API::API::OpenGL:
                return std::static_pointer_cast<Vertex_array>(std::make_shared<GL_vertex_array>());
        }
        CORE_ASSERT(false, "Unknown Renderer_API!");
        return nullptr;
    }
}

