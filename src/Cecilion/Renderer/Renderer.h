#pragma once
#include "Renderer_API.h"

namespace Cecilion {
    class Camera;
    /**
     * These are planned to be the commands that the renderer actually sends when
     * it is time to render. Currently it is done whenever submit() is called, but
     * that function should rather only "submit" something that should be rendered
     * the next frame and not actually render it immediately.
     */
    class Render_command {
    public:
        inline static void clear()  {
            s_render_API->clear();
        }
        inline static void set_clear_color(const glm::vec4& color)  {
            s_render_API->set_clear_color(color);
        }
        inline static void draw_indexed(const std::shared_ptr<Vertex_array>& vertex_array) {
            s_render_API->draw_indexed(vertex_array);
        }
        inline static void draw_instanced(const std::shared_ptr<Vertex_array>& vertex_array, int instances) {
            s_render_API->draw_instanced(vertex_array, instances);
        }
    private:
        static Renderer_API* s_render_API;
    };

    class Renderer {
    public:
        /**
         * Call this function to begin a new frame
         * @param viewport - The main camera used for rendering. This camera
         * will automatically be set as the target viewport unless any other is
         * specified.
         */
        static void begin_scene(const Camera& viewport);

        /**
         * Set the current camera used for rendering. Updating the camera after switching to it will
         * not update the viewport. This ensures that rendering a frame will not be affected by other
         * updates.
         * @param camera
         */
        static void switch_camera(const Camera& camera);
        static void end_scene();
        static void submit(const std::shared_ptr<Vertex_array>& vertex_array);
        static void submit_instanced(const std::shared_ptr<Vertex_array>& vertex_array, int instances);
        inline static Renderer_API::API get_API() {return Renderer_API::get_API();}
    };
}


