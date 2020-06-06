#include "ImGui_layer.h"

#include "ImGui_build.cpp"
//#include <Platform/OpenGL/imgui_impl_opengl3.h>
#include <Platform/OpenGL/OpenGL.h>
#include <Core/Application.h>
#include <Core/Log.h>
//#include <Event/Async_inbox.h>

// TODO Make sure the ImGui interface can handle multiple threads.
namespace Cecilion {

//    void on_mouse_button_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
//        Mouse_button_Event* click_event = dynamic_cast<Mouse_button_Event*>(message);
//        // TODO improve with double click.
//        ImGuiIO& io = ImGui::GetIO();
//        switch(click_event->action) {
//            case 0:
//                io.MouseDown[click_event->button] = false;
//                break;
//            case 1:
//                io.MouseDown[click_event->button] = true;
//                break;
//            case 2:
//                io.MouseDown[click_event->button] = true;
//                break;
//        }
//    }
//
//    void on_mouse_scroll_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
//        Mouse_scroll_Event* event = dynamic_cast<Mouse_scroll_Event*>(message);
//        ImGuiIO& io = ImGui::GetIO();
//        io.MouseWheelH += event->x_offset;
//        io.MouseWheel += event->y_offset;
//    }
//
//    void on_mouse_cursor_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
//        Mouse_cursor_Event* cursor_event = dynamic_cast<Mouse_cursor_Event*>(message);
//        ImGuiIO& io = ImGui::GetIO();
////        io.MousePos = ImVec2(cursor_event->xpos, cursor_event->ypos);
//    }
//
//    void on_key_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
//        Keyboard_key_Event* key_event = dynamic_cast<Keyboard_key_Event*>(message);
//        ImGuiIO& io = ImGui::GetIO();
//        switch (key_event->action) {
//            case 0:
//                io.KeysDown[key_event->keycode] = false;
//                break;
//            case 1:
//                io.KeysDown[key_event->keycode] = true;
//
//                io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
//                io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
//                io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
//                io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER]|| io.KeysDown[GLFW_KEY_RIGHT_SUPER];
//                break;
//            case 2:
//
//                break;
//        }
//    }
//
//    void on_char_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
//        Keyboard_char_event* char_event = dynamic_cast<Keyboard_char_event*>(message);
//        if (char_event->unicode > 0 && char_event->unicode < 0x100000) {
//            ImGuiIO& io = ImGui::GetIO();
//            io.AddInputCharacter((unsigned short)char_event->unicode);
//        }
//    }
//
//    void on_window_resize(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
//        Window_resize_event* window_event = dynamic_cast<Window_resize_event*>(message);
//        ImGuiIO& io = ImGui::GetIO();
//        io.DisplaySize = ImVec2(window_event->width, window_event->height);
//        io.DisplayFramebufferScale = ImVec2(1.0f,1.0f);
//    }

    void ImGui_layer::on_attach() {
        CORE_ASSERT(!this->p_parent, "Layer was attached without a parent!");


        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();


        Application& app = Application::get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.get_window().get_native_window());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

//    void ImGui_layer::on_detach() {
//        ImGui_ImplOpenGL3_Shutdown();
//        ImGui_ImplGlfw_Shutdown();
//        ImGui::DestroyContext();
//    }

//    void ImGui_layer::on_update() {
//        Application_layer_st::on_update();
//
//        ImGuiIO& io = ImGui::GetIO();
//        Application& app = Application::get();
//        io.DisplaySize = ImVec2(app.get_window().get_width(), app.get_window().get_height());
//
//        float time = (float)glfwGetTime();
//        io.DeltaTime = this->m_time > 0.0f ? (time - this->m_time) : (1.0f / 60.0f);
//        this->m_time = time;
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui::NewFrame();
//
//        static bool show = true;
//        ImGui::ShowDemoWindow(&show);
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//    }

    void ImGui_layer::begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGui_layer::end() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2(app.get_window().get_width(), app.get_window().get_height());

        /// Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    ImGui_layer::ImGui_layer() /*: Application_layer_st()*/ {

    }

    void ImGui_layer::on_update() {
        Layer::on_update();
        this->begin();
        this->on_imgui_render();
        this->end();
    }


//    ImGui_layer::~ImGui_layer() {
//
//    }

    void ImGui_layer::on_imgui_render() {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
}
