#include "GL_renderer_API.h"
#include "OpenGL.h"
#include <Renderer/Vertex_array.h>
#include <Renderer/Render_encoder.h>
#include <iostream>
namespace Cecilion {
    namespace Render {
        void GL_renderer_API::clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // TODO take in flags to what the renderer should clear.
        }

        void GL_renderer_API::set_clear_color(const glm::vec4 &color) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

//        void GL_renderer_API::draw_indexed(const std::shared_ptr<Vertex_array> vertex_array) {
//            glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
//        }

//        void GL_renderer_API::draw_indexed(const std::shared_ptr<Vertex_array> vertex_array, int instances) {
//            /// TODO There are some big improvements to be made here
//            glDrawElementsInstanced(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr, instances);
//        }

        void GL_renderer_API::set_depth_test(bool test) {
            if (test) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
        }

        GLuint decode_polygons(Polygon_type& polygon_type) {
            GLuint polygon;
            switch(polygon_type) {
                case Polygon_type::DOT:
                    polygon = GL_POINT;    /// TODO Check what value this should be.
                    break;
                case Polygon_type::LINE:
                    polygon = GL_LINES;
                    break;
                case Polygon_type::TRIANGLE:
                    polygon = GL_TRIANGLES;
                    break;
                case Polygon_type::QUAD:
                    polygon = GL_QUADS;
                    break;
            }
            return polygon;
        }

        void GL_renderer_API::draw_indexed(std::shared_ptr<Cecilion::Vertex_array> vertex_array,Polygon_type polygon_type, int instances) {
            GLuint polygon = decode_polygons(polygon_type);
//            const Cecilion::Vertex_array::Render_information& info = vertex_array->get_render_information();
//            switch(vertex_array->get_draw_type()) {
//                case Cecilion::Vertex_array::Render_ordering::INDEXED:
                    glDrawElementsInstanced(polygon, vertex_array->get_index_buffer()->get_count(),GL_UNSIGNED_INT,nullptr,instances);
//                    break;
//                case Cecilion::Vertex_array::Render_ordering::STREAM:
//                    glDrawArraysInstanced(polygon, info.stream.m_start_vertex,info.stream.m_vertex_count,info.stream.instances);
//                    break;
//            }
        }

        void GL_renderer_API::draw_streamed(std::shared_ptr<Cecilion::Vertex_array> vertex_array,Polygon_type polygon_type, uint32_t start_vertex,
                                            uint32_t vertex_count, uint32_t instances) {
            GLuint polygon = decode_polygons(polygon_type);
            glDrawArraysInstanced(polygon, start_vertex,vertex_count,instances);

        }

//        void GL_renderer_API::draw_streamed(std::shared_ptr<Vertex_array> vertex_array, int count, int instances) {
//            glDrawArraysInstanced(GL_TRIANGLES,0,count,instances);
//        }
    }
}