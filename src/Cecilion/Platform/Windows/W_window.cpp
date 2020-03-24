#include "W_window.h"
#include "Core/Log.h"
#include "Event/Event_system.h"

namespace Cecilion {
    #define get_GLFW_window(window) (W_window*)glfwGetWindowUserPointer(window)
    static bool s_GLFW_initialized = false;

    W_window::W_window( const Window_properties &properties) {
        Init(properties);
    }

    W_window::~W_window() {
        this->shutdown();
    }

    static void test(W_window* test) {

    }

    void W_window::Init(const Window_properties &properties) {
        this->m_data = new Window_data();
        this->m_data->title = properties.title;
        this->m_data->height = properties.height;
        this->m_data->width = properties.width;
        this->m_data->vsync = true;

        CORE_LOG_INFO("Creating window {0} ({1}, {2})", properties.title, properties.width, properties.height);

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
        glfwSetWindowUserPointer(this->m_window, this);

        glfwMakeContextCurrent(this->m_window);


        if (s_GLFW_initialized) {
            GLenum glew_success = glewInit();
            CORE_ASSERT(glew_success, "GLEW Error: No GLFW context found!")
        }

        this->set_Vsync(this->m_data->vsync);

        /// Send a shutdown event when the user tries to close the window.
        glfwSetWindowCloseCallback(this->m_window, [](GLFWwindow* window) {
            //this->post(std::make_shared<Event_message>(2));
            W_window* this_window = get_GLFW_window(window);
            Event_system::post(WINDOW_CLOSE_EVENT);
        });

        /// Resize events.
        glfwSetWindowSizeCallback(this->m_window, [](GLFWwindow* window, int width, int height) {
            W_window* this_window = get_GLFW_window(window);
            //this_window->post(std::make_s)
            this_window->m_data->height = height;
            this_window->m_data->width = width;
            Event_system::post(WINDOW_RESIZE_EVENT);
            //CORE_LOG_INFO("Window was resized to ({0}, {1})", width, height);
        });


        /// These events should be handled by the app layers.
        glfwSetMouseButtonCallback(this->m_window, [](GLFWwindow* window, int button, int action, int mods){
            //W_window* this_window = get_GLFW_window(window);
            //CORE_LOG_INFO("Button {0} set action {1}", button, action);
            Event_system::post(std::make_shared<Cecilion::Mouse_button_Event>(button, action, mods));
        });

        glfwSetKeyCallback(this->m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            //CORE_LOG_INFO("Key {0} set action {1}", key, action);
            Event_system::post(std::make_shared<Cecilion::Keyboard_Event>(key, scancode, action, mods));
        });
    }

    void W_window::set_Vsync(bool vsync) {
        // TODO enable VSYNC
    }

    bool W_window::has_Vsync() const {
        return this->m_data->vsync;
    }

    void W_window::on_update() {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1,1,0,1);
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