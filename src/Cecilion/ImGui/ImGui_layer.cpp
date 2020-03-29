#include "ImGui_layer.h"
#include <Platform/OpenGL/imgui_impl_opengl3.h>
#include <Platform/OpenGL/OpenGL.h>
#include <Core/Application.h>
#include <Core/Log.h>
#include <Event/Async_inbox.h>

// TODO Make sure the ImGui interface can handle multiple threads.
namespace Cecilion {

    void on_mouse_button_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        Mouse_button_Event* click_event = dynamic_cast<Mouse_button_Event*>(message);
        // TODO improve with double click.
        ImGuiIO& io = ImGui::GetIO();
        switch(click_event->action) {
            case 0:
                io.MouseDown[click_event->button] = false;
                break;
            case 1:
                io.MouseDown[click_event->button] = true;
                break;
            case 2:
                io.MouseDown[click_event->button] = true;
                break;
        }
    }

    void on_mouse_scroll_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        Mouse_scroll_Event* event = dynamic_cast<Mouse_scroll_Event*>(message);
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += event->x_offset;
        io.MouseWheel += event->y_offset;
    }

    void on_mouse_cursor_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        Mouse_cursor_Event* cursor_event = dynamic_cast<Mouse_cursor_Event*>(message);
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(cursor_event->xpos, cursor_event->ypos);
    }

    void on_key_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        Keyboard_key_Event* key_event = dynamic_cast<Keyboard_key_Event*>(message);
        ImGuiIO& io = ImGui::GetIO();
        switch (key_event->action) {
            case 0:
                io.KeysDown[key_event->keycode] = false;
                break;
            case 1:
                io.KeysDown[key_event->keycode] = true;

                io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
                io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
                io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
                io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER]|| io.KeysDown[GLFW_KEY_RIGHT_SUPER];
                break;
            case 2:

                break;
        }
    }

    void on_char_event(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        Keyboard_char_event* char_event = dynamic_cast<Keyboard_char_event*>(message);
        if (char_event->unicode > 0 && char_event->unicode < 0x100000) {
            ImGuiIO& io = ImGui::GetIO();
            io.AddInputCharacter((unsigned short)char_event->unicode);
        }
    }

    void on_window_resize(std::shared_ptr<I_Event_actor> actor, Event_message* message) {
        Window_resize_event* window_event = dynamic_cast<Window_resize_event*>(message);
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(window_event->width, window_event->height);
        io.DisplayFramebufferScale = ImVec2(1.0f,1.0f);
    }

    void ImGui_layer::on_attach() {
        CORE_ASSERT(!this->p_parent, "Layer was attached without a parent!");
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;


        // TODO implements Cecilion key codes.
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
        this->subscribe_to(MOUSE_BUTTON_EVENT, &on_mouse_button_event);
        this->subscribe_to(MOUSE_SCROLL_EVENT, &on_mouse_scroll_event);
        this->subscribe_to(MOUSE_CURSOR_POS_EVENT, &on_mouse_cursor_event);
        this->subscribe_to(KEYBOARD_KEY_EVENT, &on_key_event);
        this->subscribe_to(KEYBOARD_CHAR_EVENT, &on_char_event);
        this->subscribe_to(WINDOW_RESIZE_EVENT, &on_window_resize);

    }

    void ImGui_layer::on_detach() {
        // TODO Implement detach.
    }

    void ImGui_layer::on_update() {
        Application_layer_st::on_update();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2(app.get_window().get_width(), app.get_window().get_height());

        float time = (float)glfwGetTime();
        io.DeltaTime = this->m_time > 0.0f ? (time - this->m_time) : (1.0f / 60.0f);
        this->m_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    ImGui_layer::ImGui_layer() : Application_layer_st() {

    }

    ImGui_layer::~ImGui_layer() {

    }
}
