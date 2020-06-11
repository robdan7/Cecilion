#pragma  once

#include <glm/glm.hpp>
#include "Vertex_array.h"
namespace Cecilion {
    class Renderer_API {
    public:
        enum class API {
            None = 0, OpenGL = 1
        };
    public:
        virtual void clear() = 0;
        virtual void set_clear_color(const glm::vec4& color) = 0;
        virtual void draw_indexed(const std::shared_ptr<Vertex_array>& vertex_array) = 0;
        virtual void draw_instanced(const std::shared_ptr<Vertex_array>& vertex_array, int instances) = 0;
        inline static API get_API() {return s_API;}
    private:
        static API s_API;
    };
}


