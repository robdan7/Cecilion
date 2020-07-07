#include "Renderer.h"
#include "Render_command.h"
#include <Platform/OpenGL/GL_renderer_API.h>
#include "Vertex_array.h"
#include "Shader_constants.h"
#include "Camera.h"
#include "Shader.h"

namespace Cecilion {
    namespace Render {
        Renderer_API *Render_command::s_render_API = new GL_renderer_API();

        void Renderer::begin_scene(const Camera &viewport) {
            CL_System_params *params = Cecilion::get_system_params();
            params->CL_View_matrix.m_data = viewport.get_view_matrix();
            params->write((Cecilion::I_data<void *> *) &params->CL_View_matrix);

            params->CL_Projection_matrix.m_data = viewport.get_projection_matrix();
            params->write((Cecilion::I_data<void *> *) &params->CL_Projection_matrix);

            params->CL_Center_reference.m_data = viewport.get_position();
            params->write((I_data<void *> *) &params->CL_Center_reference);

            params->CL_Time.m_data = 2;
            params->write((Cecilion::I_data<void *> *) &params->CL_Time);
        }

        void Renderer::end_scene() {

        }

//        void Renderer::submit(const std::shared_ptr<Vertex_array> &vertex_array) {
//            vertex_array->bind();
//            Render_command::draw_indexed(vertex_array);
//            vertex_array->unbind();
//        }

//        void Renderer::submit_instanced(const std::shared_ptr<Cecilion::Shader> &shader,
//                                        const std::shared_ptr<Cecilion::Vertex_array> &vertex_array, int instances) {
//            shader->bind();
//            vertex_array->bind();
//            Render_command::draw_instanced(vertex_array, instances);
//            vertex_array->unbind();
//            shader->unbind();
//        }

        void Renderer::switch_camera(const Camera &camera) {
            CL_System_params *params = Cecilion::get_system_params();
            params->CL_View_matrix.m_data = camera.get_view_matrix();
            params->write((Cecilion::I_data<void *> *) &params->CL_View_matrix);

            params->CL_Projection_matrix.m_data = camera.get_view_projection_matrix();
            params->write((Cecilion::I_data<void *> *) &params->CL_Projection_matrix);
        }

        void Renderer::submit_streamed(const std::shared_ptr<Cecilion::Shader>& shader,std::shared_ptr<Cecilion::Vertex_array> vertex_array, Polygon_type polygon_type,
                                       uint32_t start_vertex, uint32_t vertex_count, uint32_t instances) {
            shader->bind();
            vertex_array->bind();
            Render_command::draw_stream(vertex_array,polygon_type,start_vertex,vertex_count,instances);
            vertex_array->unbind();
            shader->unbind();
        }

        void Renderer::submit_indexed(const std::shared_ptr<Cecilion::Shader> &shader,
                                      std::shared_ptr<Cecilion::Vertex_array> vertex_array, Polygon_type polygon_type,
                                      int instances) {
            shader->bind();
            vertex_array->bind();
            Render_command::draw_indexed(vertex_array,polygon_type,instances);
            vertex_array->unbind();
            shader->unbind();

        }

//        void Renderer::submit_arrays(const std::shared_ptr<Cecilion::Shader> &shader,
//                                const std::shared_ptr<Cecilion::Vertex_array> &vertex_array,
//                                int vertices, int instances) {
//            shader->bind();
//            vertex_array->bind();
//            Render_command::draw_streams_instanced(vertex_array, vertices, instances);
//            vertex_array->unbind();
//            shader->unbind();
//        }
    }

}
