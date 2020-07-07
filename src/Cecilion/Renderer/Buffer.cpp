#include "Buffer.h"
#include "Renderer.h"
#include <Core/Log.h>
#include <Platform/OpenGL/GL_buffer.h>
namespace Cecilion {

    /**
     * Create a new vertex buffer. There is no need to create an API specific buffer.
     * @param vertices
     * @param size - The size in bytes.
     * @return
     */
    std::shared_ptr<Vertex_buffer> Cecilion::Vertex_buffer::Create(void* vertices, uint32_t size, Raw_buffer::Access_frequency frequency, Raw_buffer::Access_type type) {
        switch (Render::Renderer::get_API()) {
            case Render::Renderer_API::API::None :
                CORE_ASSERT(false, "Renderer::Buffer:: No render API is defined!");
                break;
            case Render::Renderer_API::API::OpenGL:
                return std::static_pointer_cast<Vertex_buffer>(std::make_shared<GL_vertex_buffer>(vertices, size,frequency,type));
        }
        CORE_ASSERT(false, "Unknown Renderer_API!");
        return nullptr;
    }

    /**
     * Create a new index buffer. There is no need to crete an API specific buffer.
     * @param indices
     * @param count
     * @return
     */
    std::shared_ptr<Index_buffer> Cecilion::Index_buffer::Create(uint32_t *indices, uint32_t count) {
        switch (Render::Renderer::get_API()) {
            case Render::Renderer_API::API::None :
                CORE_ASSERT(false, "Renderer::Buffer:: No render API is defined!");
                break;
            case Render::Renderer_API::API::OpenGL:
                return std::static_pointer_cast<Index_buffer>(std::make_shared<GL_index_buffer>(indices, count));
        }
        CORE_ASSERT(false, "Unknown Renderer_API!");
        return nullptr;
    }

    std::shared_ptr<Constant_buffer>
    Constant_buffer::Create(void *vertices, uint32_t size, Raw_buffer::Access_frequency frequency,
                            Raw_buffer::Access_type type) {
        switch (Render::Renderer::get_API()) {
            case Render::Renderer_API::API::None :
                CORE_ASSERT(false, "Renderer::Buffer:: No render API is defined!");
                break;
            case Render::Renderer_API::API::OpenGL:
                return std::static_pointer_cast<Constant_buffer>(std::make_shared<GL_constant_buffer>(vertices, size, frequency, type));
        }
        CORE_ASSERT(false, "Unknown Renderer_API!");
        return nullptr;
    }
}

