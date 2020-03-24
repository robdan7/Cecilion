#pragma once
#define CECILION_PLATFORM_WINDOWS
#include <string>

namespace Cecilion {
    struct Window_properties {
        std::string title;
        unsigned int width, height;

        Window_properties(
                const std::string &title = "Cecilion engine",
                unsigned int width = 1080,
                unsigned int height = 720) :
                title(title), width(width), height(height){}
    };

    class Window {
    public:
        virtual ~Window() {}
        virtual void on_update() = 0;
        virtual unsigned int get_height() const = 0;
        virtual unsigned int get_width() const = 0;
        virtual void set_Vsync(bool vsync) = 0;
        virtual bool has_Vsync() const = 0;
        static Window* create_window(const Window_properties& properties = Window_properties());
    };
}