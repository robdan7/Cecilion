#include "Renderer.h"
#include "Render_command.h"
#include <Platform/OpenGL/GL_renderer_API.h>
#include "Vertex_array.h"

namespace Cecilion {
    Renderer_API* Render_command::s_render_API = new GL_renderer_API();

    void Renderer::begin_scene() {

    }

    void Renderer::end_scene() {

    }

    void Renderer::submit(const std::shared_ptr<Vertex_array> &vertex_array) {
        vertex_array->bind();
        Render_command::draw_indexed(vertex_array);
        vertex_array->unbind();
    }

    void Renderer::submit_instanced(const std::shared_ptr<Vertex_array> &vertex_array, int instances) {
        vertex_array->bind();
        Render_command::draw_instanced(vertex_array, instances);
        vertex_array->unbind();
    }
}
