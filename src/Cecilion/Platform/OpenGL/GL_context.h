#pragma once
#include <Renderer/GLFW_context.h>
namespace Cecilion {
    class GL_context : public GLFW_context{
    public:
        void init(GLFWwindow* window) override;

        void swap_buffers() override;
    };
}

