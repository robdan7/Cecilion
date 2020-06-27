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
        CORE_LOG_INFO("Created window {0} ({1}, {2})", properties.title, properties.width, properties.height);

        this->m_graphics_context->init(this->m_window);

        const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
        const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
        const GLubyte* version = glGetString(GL_VERSION);
        CORE_LOG_INFO("Platform::Windows::W_window:: Created graphics context.\n    Vendor: {0}\n    Model: {1}\n    Version: {2}", (char*)vendor,(char*)renderer, (char*)version);

        glfwSetWindowUserPointer(this->m_window, this);
        this->set_Vsync(this->m_data->vsync);

        /// Send a shutdown event when the user tries to close the window.
        glfwSetWindowCloseCallback(this->m_window, [](GLFWwindow* window) {
            //this->post(std::make_shared<Event_message>(2));
            W_window* this_window = get_GLFW_window(window);
            Event_system::post<Cecilion::Window_close_event>();
        });

        /// Resize events.
        glfwSetWindowSizeCallback(this->m_window, [](GLFWwindow* window, int width, int height) {
            W_window* this_window = get_GLFW_window(window);
            //this_window->post(std::make_s)
            this_window->m_data->height = height;
            this_window->m_data->width = width;
            this_window->activate_resize();
            Event_system::post<Cecilion::Window_resize_event>(width, height);
        });


        /// These events should be handled by the app layers.
        glfwSetMouseButtonCallback(this->m_window, [](GLFWwindow* window, int button, int action, int mods){
            Event_system::post<Cecilion::Mouse_button_Event>(button, action);
        });

        glfwSetKeyCallback(this->m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            Event_system::post<Cecilion::Keyboard_key_Event>(key, action);
        });

        glfwSetCharCallback(this->m_window, [](GLFWwindow* window, unsigned int unicode) {
            Event_system::post<Keyboard_char_event>(unicode);
        });

        glfwSetCursorPosCallback(this->m_window, [] (GLFWwindow* window, double xpos, double ypos) {
            Event_system::post<Cecilion::Mouse_cursor_Event>(xpos, ypos);
        });

        glfwSetScrollCallback(this->m_window,  [] (GLFWwindow* window, double xoffset, double yoffset){
            Event_system::post<Cecilion::Mouse_scroll_Event>(xoffset, yoffset);
        });

    }

    void W_window::set_Vsync(bool vsync) {
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        this->m_data->vsync = vsync;
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

    void W_window::show_cursor() {
        glfwSetInputMode(this->m_window,GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void W_window::hide_cursor() {
        glfwSetInputMode(this->m_window,GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void W_window::disable_cursor() {
        glfwSetInputMode(this->m_window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

}