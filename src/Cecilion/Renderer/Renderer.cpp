#include "Renderer.h"
#include "Render_command.h"

namespace Cecilion {

    void Renderer::begin_scene() {

    }

    void Renderer::end_scene() {

    }

    void Renderer::submit(const std::shared_ptr<Vertex_array> &vertex_array) {
        vertex_array->bind();
        Render_command::draw_indexed(vertex_array);
        vertex_array->unbind();
    }
}
