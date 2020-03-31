#pragma once

#include <Core/Layers/Application_layer_st.h>
namespace Cecilion {
    class ImGui_layer : public Application_layer_st {
    public:
        ImGui_layer();
        ~ImGui_layer() override;
        void on_attach() override;
        void on_detach() override;
//        void on_update() override;
        void on_imgui_render() override;

        void begin();
        void end();
    private:

        float m_time = 0.0f;
    };
}


