#include <Platform/OpenGL/GL_shader.h>
#include "Renderer.h"

namespace Cecilion {

    std::shared_ptr<Shader_stage> Shader::create_shader_stage(const uint32_t shader_type, const std::string&& shader_source) {
        switch(Renderer::get_API()) {
            case Renderer_API::API::None:
                CORE_ASSERT(false, "Cecilion::Shader:: No render API is defined!");
                break;
            case Renderer_API::API::OpenGL:
                return std::make_shared<GL_shader_stage>(shader_type, std::move(shader_source));
                break;
        }
        return nullptr;
    }

    std::shared_ptr<Shader> Shader::create_shader(const std::initializer_list<std::shared_ptr<Shader_stage>> elements) {
        switch (Renderer::get_API()) {
            case Renderer_API::API::None:
                CORE_ASSERT(false, "Cecilion::Shader:: No render API is defined!");
                break;
            case Renderer_API::API::OpenGL:
                return std::make_shared<GL_shader>(std::move(elements));
                break;

        }
        return nullptr;
    }

    std::shared_ptr<Shader> Shader::load_binary_shader(uint32_t binary_format, const char *binary, uint32_t size) {
        switch (Renderer::get_API()) {
            case Renderer_API::API::None:
                CORE_ASSERT(false, "Cecilion::Shader:: No render API is defined!");
                break;
            case Renderer_API::API::OpenGL:
                return std::make_shared<GL_shader>(binary_format, binary, size);
                break;

        }
        return nullptr;
    }
}
