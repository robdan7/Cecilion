#pragma once

#include "Renderer_API.h"
namespace Cecilion {

    class Renderer {
    public:
        static void begin_scene();
        static void end_scene();
        static void submit(const std::shared_ptr<Vertex_array>& vertex_array);
        inline static Renderer_API::API get_API() {return Renderer_API::get_API();}
    };
}


