#pragma once

#include <Core/Input.h>
namespace Cecilion {
    class Windows_input : public Input {
    protected:
        bool get_key_press_status(int keycode) override;
        bool get_mouse_button_status(int button) override;
        std::pair<float, float> get_mouse_pos_position() override;
    };
}

