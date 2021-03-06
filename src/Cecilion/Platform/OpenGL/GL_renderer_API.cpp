#include "GL_renderer_API.h"
#include "OpenGL.h"
#include <Renderer/Vertex_array.h>
namespace Cecilion {

    void GL_renderer_API::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO take in flags to what the renderer should clear.
    }

    void GL_renderer_API::set_clear_color(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GL_renderer_API::draw_indexed(const std::shared_ptr<Vertex_array> vertex_array) {
        glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
    }

    void GL_renderer_API::draw_instanced(const std::shared_ptr<Vertex_array> vertex_array, int instances) {
        /// TODO There are some big improvements to be made here
        glDrawElementsInstanced(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr, instances);
    }

}