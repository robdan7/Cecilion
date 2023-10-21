#include "Input.h"
#include <Event/Event_system.h>
namespace Cecilion {
    /**
  * Notify a keyboard key. This method will be called for all types of keys, e.g. ALT, ENTER or SHIFT.
  * @param keycode
  * @param action
  */
    void Input::notify_key_status(int keycode, int action) {
        Event_system::post<Cecilion::Events::Keyboard_key_Event>(keycode,action);
    }

/**
 * Notify a char key status. This is only used for letters such as A, B of C.
 * This function should only be called when a new char input typed or held down, not released.
 * @param keycode
 */
    void Input::notify_char_status(int keycode) {
        Event_system::post<Cecilion::Events::Keyboard_char_event>(keycode);
    }

/**
 * Notify the mouse position. Simple as that.
 * @param x
 * @param y
 */
    void Input::notify_mouse_pos(double x, double y) {
        Event_system::post<Cecilion::Events::Mouse_cursor_Event>(x, y);
    }

/**
 * Notify mouse button status.
 * @param button
 */
    void Input::notify_mouse_button(int button, int action) {
            Event_system::post<Cecilion::Events::Mouse_button_Event>(button, action);
    }

/**
 * Notify mouse scroll.
 * @param x_offset
 * @param y_offset
 */
    void Input::notify_mouse_scroll(double x_offset, double y_offset) {
        Event_system::post<Cecilion::Events::Mouse_scroll_Event>(x_offset, y_offset);
    }

// TODO Implement support for generic controllers. This should not be different from mouse input.
}


