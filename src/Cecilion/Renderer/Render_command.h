#pragma once

#include "Renderer_API.h"
namespace Cecilion {
    class Render_command {
    public:
        inline static void clear()  {
            s_render_API->clear();
        }
        inline static void set_clear_color(const glm::vec4& color)  {
            s_render_API->set_clear_color(color);
        }
        inline static void draw_indexed(const std::shared_ptr<Vertex_array>& vertex_array) {
            s_render_API->draw_indexed(vertex_array);
        }
    private:
        static Renderer_API* s_render_API;
    };
}
