#pragma once

#include <Core/Layers/Layer.h>
//#include <Core/Layers/Application_layer_st.h>

namespace Cecilion {
    class ImGui_layer : public Layer<> {
    public:
        ImGui_layer();
        void on_attach() override;
        void on_update() override;
//        ~ImGui_layer() override;
////        void on_detach() override;


        void begin();
        void end();
        void on_imgui_render();
    private:

        float m_time = 0.0f;
    };
}


