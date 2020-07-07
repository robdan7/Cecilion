#include "Render_encoder.h"
#include "Renderer.h"
namespace Cecilion {
    namespace Render {

        void Stream_renderer::draw_instanced(const std::shared_ptr<Cecilion::Shader>& shader,const std::shared_ptr<Cecilion::Vertex_array> &vertex_array, uint32_t instances) const {
            Renderer::submit_streamed(shader, vertex_array,this->polygons, this->start_vertex,this->vertex_count,instances);
        }

        Stream_renderer::Stream_renderer(Polygon_type polygon_type, uint32_t start_vertex, uint32_t vertex_count) : Render_encoder(polygon_type) {
            this->vertex_count = vertex_count;
            this->start_vertex = start_vertex;
        }

        void Index_renderer::draw_instanced(const std::shared_ptr<Cecilion::Shader>& shader,const std::shared_ptr<Cecilion::Vertex_array> &vertex_array, uint32_t instances) const {
            Renderer::submit_indexed(shader, vertex_array,this->polygons,instances);
        }
    }
}