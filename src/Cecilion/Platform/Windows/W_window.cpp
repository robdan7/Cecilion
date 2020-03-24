#include "W_window.h"
#include "Core/Log.h"

namespace Cecilion {
    static bool s_GLFW_initialized = false;

    W_window::W_window(const Window_properties& properties) {
        Init(properties);
    }

    W_window::~W_window() {
        this->shutdown();
    }

    void W_window::Init(const Window_properties &properties) {
        this->m_data = new Window_data();
        this->m_data->title = properties.title;
        this->m_data->height = properties.height;
        this->m_data->width = properties.width;
        this->m_data->vsync = true;

        CORE_LOG_INFO("Creating wandow {0} ({1}, {2})", properties.title, properties.width, properties.height);

        if (!s_GLFW_initialized) {
            int glfw_success = glfwInit();
            CORE_ASSERT(glfw_success, "GLFW Error: Could not initialize!");
            s_GLFW_initialized = true;
        }

        this->m_window = glfwCreateWindow(
                (int)this->m_data->width,
                (int)this->m_data->height,
                this->m_data->title.c_str(),
                NULL, NULL);

        glfwMakeContextCurrent(this->m_window);


        if (s_GLFW_initialized) {
            GLenum glew_success = glewInit();
            CORE_ASSERT(glew_success, "GLEW Error: No GLFW context found!")
        }

        this->set_Vsync(this->m_data->vsync);
    }

    void W_window::set_Vsync(bool vsync) {
        // TODO enable VSYNC
    }

    bool W_window::has_Vsync() const {
        return this->m_data->vsync;
    }

    void W_window::on_update() {
        glfwPollEvents();
        glfwSwapBuffers(this->m_window);
    }

    void W_window::shutdown() {
        glfwDestroyWindow(this->m_window);
        if (s_GLFW_initialized) {
            glfwTerminate();
        }
    }

}