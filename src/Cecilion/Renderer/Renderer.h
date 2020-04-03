#pragma once
namespace Cecilion {
    enum class Renderer_API {
        None = 0, OpenGL = 1
    };

    class Renderer {
    public:
        inline static Renderer_API get_API() {return s_renderer_API;}
        static Renderer_API s_renderer_API;
    };
}


