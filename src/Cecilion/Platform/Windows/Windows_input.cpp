#include "Windows_input.h"
#include <Core/Application.h>
#include <Platform/OpenGL/OpenGL.h>

namespace Cecilion {
    Input* Input::s_instance = new Windows_input();

    bool Cecilion::Windows_input::get_key_press_status(int keycode) {
        auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
        auto state =  glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Cecilion::Windows_input::get_mouse_button_status(int button) {
        auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
        auto state =  glfwGetMouseButton(window, button);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    std::pair<float, float> Cecilion::Windows_input::get_mouse_pos() {
        auto window = static_cast<GLFWwindow*>(Application::get().get_window().get_native_window());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return std::pair<float, float>(x,y);
    }
}

