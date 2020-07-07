#pragma once
#include <Renderer/Renderer_API.h>
#include <glm/glm.hpp>

namespace Cecilion {
    namespace Render {
        class GL_renderer_API : public Renderer_API{
        public:
            void clear() override;
            void set_clear_color(const glm::vec4 &color) override;
//            void draw_indexed(const std::shared_ptr<Vertex_array> vertex_array) override;

//            void draw_indexed(const std::shared_ptr<Vertex_array> vertex_array, int instances) override;

//            void draw_streamed(std::shared_ptr<Vertex_array> vertex_array, int count, int instances) override;
            void draw_indexed(std::shared_ptr<Cecilion::Vertex_array> vertex_array,Polygon_type polygon_type, int instances) override;

            void draw_streamed(std::shared_ptr<Cecilion::Vertex_array> vertex_array, Polygon_type polygon_type, uint32_t start_vertex,
                               uint32_t vertex_count, uint32_t instances) override;

            void set_depth_test(bool test) override;
        };
    }
}


