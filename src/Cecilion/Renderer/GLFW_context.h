#pragma once

class GLFWwindow;
class GLFW_context {
public:
    virtual void swap_buffers() = 0;
    virtual void init(GLFWwindow* window) = 0;

protected:
    GLFWwindow* m_window;
};
