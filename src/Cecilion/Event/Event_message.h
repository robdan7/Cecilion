#pragma once
namespace Cecilion {
    const unsigned int SYSTEM_SHUTDOWN_EVENT  = __COUNTER__;
    const unsigned int SYSTEM_STARTUP_EVENT = __COUNTER__;
    const unsigned int SYSTEM_TIMER_EVENT = __COUNTER__;
    const unsigned int WINDOW_CLOSE_EVENT = __COUNTER__;
    const unsigned int WINDOW_RESIZE_EVENT = __COUNTER__;
    const unsigned int MOUSE_BUTTON_EVENT = __COUNTER__;
    const unsigned int MOUSE_SCROLL_EVENT = __COUNTER__;
    const unsigned int MOUSE_CURSOR_POS_EVENT = __COUNTER__;
    const unsigned int KEYBOARD_KEY_EVENT = __COUNTER__;
    const unsigned int KEYBOARD_CHAR_EVENT = __COUNTER__;
    const unsigned int TIME_STOP_EVENT = __COUNTER__;
    const unsigned int TIME_START_EVENT = __COUNTER__;
    const unsigned int AUDIO_PLAY_EVENT = __COUNTER__;
    const unsigned int AUDIO_PAUSE_EVENT = __COUNTER__;
    const unsigned int AUDIO_VOLUME_EVENT = __COUNTER__;

    struct Event_message {
        unsigned int message_ID;
        Event_message(unsigned int ID) : message_ID(ID) {}
        virtual ~Event_message() = default;
    };

    struct Mouse_button_Event : public Event_message {
        const int button, action, mods;
        Mouse_button_Event(int button, int action, int mods) : Event_message(MOUSE_BUTTON_EVENT), button(button), action(action), mods(mods) {
        }
    };

    struct Mouse_cursor_Event : public Event_message {
        const double xpos, ypos;
        Mouse_cursor_Event(double xpos, double ypos) : Event_message(MOUSE_CURSOR_POS_EVENT), xpos(xpos), ypos(ypos) {
        }
    };

    struct Mouse_scroll_Event : public Event_message {
        const double x_offset, y_offset;
        Mouse_scroll_Event(double x_offset, double y_offset) : Event_message(MOUSE_SCROLL_EVENT), x_offset(x_offset), y_offset(y_offset) {}
    };

    struct Keyboard_key_Event : public Event_message {
        const int key, scancode, action, mods;
        Keyboard_key_Event(int key, int scancode, int action, int mods) : Event_message(KEYBOARD_KEY_EVENT), key(key), scancode(scancode), action(action), mods(mods) {}
    };

    struct Keyboard_char_event : public Event_message {
        unsigned int unicode;
        Keyboard_char_event(unsigned int unicode) : Event_message(KEYBOARD_CHAR_EVENT), unicode(unicode){}
    };

    struct Window_resize_event : public Event_message {
        int width, height;
        Window_resize_event(int width, int height) : Event_message(WINDOW_RESIZE_EVENT), width(width), height(height){}
    };
}

