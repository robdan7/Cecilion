#pragma once

#include <typeindex>
namespace Cecilion {
    /// There are some basic event types supported by the engine.

    struct Mouse_button_Event {
        int button, action;
        Mouse_button_Event(int button, int action) : button(button), action(action) {
        }
    };

    struct Mouse_cursor_Event {
        double xpos, ypos;
        Mouse_cursor_Event(double xpos, double ypos) : xpos(xpos), ypos(ypos) {
        }
    };

    struct Mouse_scroll_Event {
        double x_offset, y_offset;
        Mouse_scroll_Event(double x_offset, double y_offset) : x_offset(x_offset), y_offset(y_offset) {}
    };

    struct Keyboard_key_Event {
        int keycode, action;
        Keyboard_key_Event(int keycode, int action) : keycode(keycode), action(action) {}
    };

    struct Keyboard_char_event {
        unsigned int unicode;
        Keyboard_char_event(unsigned int unicode) : unicode(unicode){}
    };

    struct Window_resize_event {
        int width, height;
        Window_resize_event(int width, int height) : width(width), height(height){}
    };

    struct Window_close_event {
        Window_close_event() {}
    };
}