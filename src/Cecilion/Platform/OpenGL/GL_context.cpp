//
// Created by Robin on 2020-03-31.
//

#include "GL_context.h"
#include "OpenGL.h"
#include <Core/Log.h>
namespace Cecilion {
    void GL_context::swap_buffers() {
        glfwSwapBuffers(this->m_window);
    }

    void GL_context::init(GLFWwindow* window) {
        this->m_window = window;
        glfwMakeContextCurrent(window);

        GLenum glew_success = glewInit();
        CORE_ASSERT(glew_success, "GLEW Error: No GLFW context found!");
    }
}
