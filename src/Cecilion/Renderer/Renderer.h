#pragma once
#include "Renderer_API.h"

namespace Cecilion {
    class Camera;
    class Shader;
    namespace Render {
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
//            inline static void draw_indexed(const std::shared_ptr<Cecilion::Vertex_array>& vertex_array) {
//                s_render_API->draw_indexed(vertex_array);
//            }
//            inline static void draw_instanced(const std::shared_ptr<Cecilion::Vertex_array>& vertex_array, int instances) {
//                s_render_API->draw_indexed(vertex_array, instances);
//            }
//            inline static void draw_streams_instanced(const std::shared_ptr<Cecilion::Vertex_array>& vertex_array, int count, int instances) {
//                s_render_API->draw_streamed(vertex_array,count,instances);
//            }
            inline static void draw_stream(std::shared_ptr<Cecilion::Vertex_array> vertex_array, Polygon_type polygon_type, uint32_t start_vertex, uint32_t vertex_count, uint32_t instances) {
                s_render_API->draw_streamed(vertex_array,polygon_type,start_vertex,vertex_count,instances);
            }
            inline static void draw_indexed(std::shared_ptr<Cecilion::Vertex_array> vertex_array,Polygon_type polygon_type, int instances) {
                s_render_API->draw_indexed(vertex_array,polygon_type,instances);
            }
            inline static void enable_depth_test() {
                s_render_API->set_depth_test(true);
            }
            inline static void disable_depth_test() {
                s_render_API->set_depth_test(false);
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
//            static void submit(const std::shared_ptr<Cecilion::Vertex_array>& vertex_array);
//            static void submit_instanced(const std::shared_ptr<Cecilion::Shader>& shader, const std::shared_ptr<Cecilion::Vertex_array>& vertex_array, int instances);
//            static void submit_arrays(const std::shared_ptr<Cecilion::Shader>& shader, const std::shared_ptr<Cecilion::Vertex_array>& vertex_array, int vertices, int instances);
            static void submit_streamed(const std::shared_ptr<Cecilion::Shader>& shader,std::shared_ptr<Cecilion::Vertex_array> vertex_array, Polygon_type polygon_type, uint32_t start_vertex, uint32_t vertex_count, uint32_t instances);
            static void submit_indexed(const std::shared_ptr<Cecilion::Shader>& shader,std::shared_ptr<Cecilion::Vertex_array> vertex_array,Polygon_type polygon_type, int instances);
            inline static Renderer_API::API get_API() {return Renderer_API::get_API();}
        };
    }
}


