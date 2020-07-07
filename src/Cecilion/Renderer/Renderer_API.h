#pragma  once

#include <glm/glm.hpp>
#include <memory>
//#include "Vertex_array.h"
namespace Cecilion {
    class Vertex_array;
    namespace Render {
        enum class Polygon_type;
        class Renderer_API {
        public:
            enum class API {
                None = 0, OpenGL = 1
            };
        public:
            virtual void clear() = 0;
            virtual void set_clear_color(const glm::vec4& color) = 0;
//            virtual void draw_indexed(const std::shared_ptr<Cecilion::Vertex_array> vertex_array) = 0;
//            virtual void draw_indexed(const std::shared_ptr<Cecilion::Vertex_array> vertex_array, int instances) = 0;
//            virtual void draw_streamed(std::shared_ptr<Cecilion::Vertex_array> vertex_array, int count, int instances) = 0;
            virtual void draw_indexed(std::shared_ptr<Cecilion::Vertex_array> vertex_array,Polygon_type polygon_type, int instances) = 0;
            virtual void draw_streamed(std::shared_ptr<Cecilion::Vertex_array> vertex_array, Polygon_type polygon_type, uint32_t start_vertex, uint32_t vertex_count, uint32_t instances) = 0;
            inline static API get_API() {return s_API;}
            virtual void set_depth_test(bool test)=0;
        private:
            static API s_API;
        };
    }
}


