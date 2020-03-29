#pragma once

#include <utility>

namespace Cecilion {
    /**
     * Global input class for Cecilion. Use polling to get the status of a few keys,
     * and event subscriptions - such as KEYBOARD_CHAR_EVENT - if you want to track the status of
     * a lot of keys that might not change state the whole time.
     *
     * The system does not support multiple windows at the time. But the rest of Cecilion doesn't either,
     * so it's no biggie.
     *
     * TODO implement input from multiple windows. Require multiple window targets.
     */
    class Input {
    public:
        inline static bool is_key_pressed(int keycode) {return s_instance->get_key_press_status(keycode);}
        inline static bool is_mouse_button_pressed(int button) {return s_instance->get_mouse_button_status(button);}
        inline static std::pair<float, float> get_mouse_position() {return s_instance->get_mouse_pos_position();}
        static void notify_key_status(int keycode, int action);
        static void notify_char_status(int keycode);
        static void notify_mouse_pos(double x, double y);
        static void notify_mouse_button(int button, int action);
        static void notify_mouse_scroll(double x_offset, double y_offset);
    protected:
        virtual bool get_key_press_status(int keycode) = 0;
        virtual bool get_mouse_button_status(int button) = 0;
        virtual std::pair<float, float> get_mouse_pos_position() = 0;
    private:
        // TODO Implement keycodes.
        static int keycodes[100];
        static int mouse_buttons[100];
        static Input* s_instance;
    };
}
