#include "Buffer.h"
#include "Renderer.h"
#include <Core/Log.h>
#include <Platform/OpenGL/GL_buffer.h>
namespace Cecilion {
    Vertex_buffer *Cecilion::Vertex_buffer::Create(float *vertices, uint32_t size) {
        switch (Renderer::get_API()) {
            case Renderer_API::API::None :
                CORE_ASSERT(false, "Renderer_API:: No render API is defined!");
                break;
            case Renderer_API::API::OpenGL:
                return new GL_vertex_buffer(vertices, size);
        }
        CORE_ASSERT(false, "Unknown Renderer_API!");
        return nullptr;
    }

    Index_buffer *Cecilion::Index_buffer::Create(uint32_t *indices, uint32_t count) {
        switch (Renderer::get_API()) {
            case Renderer_API::API::None :
                CORE_ASSERT(false, "Renderer_API:: No render API is defined!");
                break;
            case Renderer_API::API::OpenGL:
                return new GL_index_buffer(indices, count);
        }
        CORE_ASSERT(false, "Unknown Renderer_API!");
        return nullptr;
    }
}

