#include "W_window.h"
#include <Core/Log.h>
#include <Event/Event_system.h>
#include <Core/Input.h>


namespace Cecilion {
    #define get_GLFW_window(window) (W_window*)glfwGetWindowUserPointer(window)
    static bool s_GLFW_initialized = false;

    W_window::W_window( const Window_properties &properties, GLFW_context* context) {
        Init(properties, context);
    }

    W_window::~W_window() {
        this->shutdown();
    }

    static void test(W_window* test) {

    }

    void W_window::Init(const Window_properties &properties, GLFW_context* context) {
        this->m_graphics_context = context;
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

        this->m_graphics_context->init(this->m_window);
//        glfwMakeContextCurrent(this->m_window);

//        GLenum glew_success = glewInit();
//        CORE_ASSERT(glew_success, "GLEW Error: No GLFW context found!");

        glfwSetWindowUserPointer(this->m_window, this);
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
            this_window->activate_resize();
            Event_system::post(std::make_shared<Window_resize_event>(width, height));
            //CORE_LOG_INFO("Window was resized to ({0}, {1})", width, height);
        });


        /// These events should be handled by the app layers.
        glfwSetMouseButtonCallback(this->m_window, [](GLFWwindow* window, int button, int action, int mods){
            //W_window* this_window = get_GLFW_window(window);
            Event_system::post(std::make_shared<Cecilion::Mouse_button_Event>(button, action));
//            Input::notify_mouse_button(button, action);
        });

        glfwSetKeyCallback(this->m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            //CORE_LOG_INFO("Key {0} set action {1}", key, action);
            Event_system::post(std::make_shared<Cecilion::Keyboard_key_Event>(key, action));
//            Input::notify_key_status(key, action);
        });

        glfwSetCharCallback(this->m_window, [](GLFWwindow* window, unsigned int unicode) {
            Event_system::post(std::make_shared<Keyboard_char_event>(unicode));
        });

        glfwSetCursorPosCallback(this->m_window, [] (GLFWwindow* window, double xpos, double ypos) {
            Event_system::post(std::make_shared<Cecilion::Mouse_cursor_Event>(xpos, ypos));
        });

        glfwSetScrollCallback(this->m_window,  [] (GLFWwindow* window, double xoffset, double yoffset){
            Event_system::post(std::make_shared<Cecilion::Mouse_scroll_Event>(xoffset, yoffset));
        });

    }

    void W_window::set_Vsync(bool vsync) {
        // TODO enable VSYNC
    }

    bool W_window::has_Vsync() const {
        return this->m_data->vsync;
    }

    void W_window::on_update() {
        //glClear(GL_COLOR_BUFFER_BIT);
        //glClearColor(1,1,0,1);
        glfwPollEvents();
        // TODO Render context.
        this->m_graphics_context->swap_buffers();
        //glfwSwapBuffers(this->m_window);
    }

    void W_window::shutdown() {
        glfwDestroyWindow(this->m_window);
        if (s_GLFW_initialized) {
            glfwTerminate();
        }
    }

    void W_window::activate_resize() {
        glViewport(0,0, this->m_data->width, this->m_data->height);
    }

}