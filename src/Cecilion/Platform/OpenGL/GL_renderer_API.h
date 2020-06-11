#pragma once
#include <Renderer/Renderer_API.h>
#include <glm/glm.hpp>

namespace Cecilion {
    class GL_renderer_API : public Renderer_API{
    public:
        void clear() override;
        void set_clear_color(const glm::vec4 &color) override;
        void draw_indexed(const std::shared_ptr<Vertex_array> &vertex_array) override;

        void draw_instanced(const std::shared_ptr<Vertex_array> &vertex_array, int instances) override;
    };
}


