#include "GL_Frustum_compute_query.h"
#include <Platform/OpenGL/GL_shader.h>
#include <Platform/OpenGL/GL_vertex_array.h>
#include <Platform/OpenGL/OpenGL.h>
#include <Debug/Instrumentor.h>

namespace Cecilion {

    GL_Frustum_compute_query::~GL_Frustum_compute_query() {

    }

    void GL_Frustum_compute_query::execute() {
        Frustum_compute_query::execute();
        this->shader_program->bind();
        this->m_vao->bind();

        glGenQueries(1,&m_last_frustum_query);
        glEnable(GL_RASTERIZER_DISCARD);
        glBeginTransformFeedback(GL_POINTS);
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_last_frustum_query);
        glDrawArrays(GL_POINTS, 0, this->m_vertices->get_size() / this->m_vertices->get_layout().get_stride());
        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
//            glFlush();  // Make sure all commands are transported into the pipeline.

        this->m_vao->unbind();
        this->shader_program->unbind();
        this->m_has_updated = true;
    }

    GL_Frustum_compute_query::GL_Frustum_compute_query(std::shared_ptr<Vertex_buffer> data_buffer, std::shared_ptr<Vertex_buffer> instance_data)
            : Frustum_compute_query(data_buffer, instance_data) {

        /*
         * TODO The shaders are not actually calculating whether the vertex is inside the screen,
         * it just checks whether the position is inside (1,1). There are also no view matrices transform
         * the position into screen space.
         */

        std::string vertex_shader = R"(
            #version 440 core
            in vec4 in_position;
//            in float in_radius;

            out Vertex_data {
                vec4 position;
//                float radius;
            } vertex_data;

        )";

        std::string data[this->m_result_buffer->get_layout().get_elements().size()];
        std::vector<Buffer_element>::const_iterator iterator = this->m_result_buffer->get_layout().begin();
        int i = 0;
        while (iterator != this->m_result_buffer->get_layout().end()) {
            data[i] = iterator->m_string_representation;
            data[i] += " instance_" + iterator->m_name;
            vertex_shader += "in " + data[i] + ";\n";
            iterator++;
            i++;

        }
        vertex_shader += "out Instance_data {\n";
        for (int x = 0; x < i; x++) {
            vertex_shader += data[x] + ";\n";
        }
        vertex_shader += R"(
            } instance_output;
            void main() {
                vertex_data.position = in_position;
//                vertex_data.radius = in_radius;
            )";
        iterator = this->m_result_buffer->get_layout().begin();
        while (iterator != this->m_result_buffer->get_layout().end()) {
            vertex_shader += "instance_output.instance_" + iterator->m_name + " = instance_"+iterator->m_name + ";\n";
            iterator ++;
        }
        vertex_shader += "}";
//        CORE_LOG_INFO(vertex_shader);
        auto vertex_stage = std::make_shared<GL_shader_stage>(OPENGL_VERTEX_SHADER, std::move(vertex_shader));

        /// -------- Geometry shader ---------
        std::string geometry = R"(
            #version 440 core
            layout (points) in;
            layout (points, max_vertices = 1) out;

            in Vertex_data {
                vec4 position;
//                float radius;
            } vertex_data[];

        )";

        geometry += "in Instance_data {\n";
        for (int x = 0; x < i; x++) {
            geometry += data[x] + ";\n";
        }
        geometry += "} instance_output[];\n";

        geometry += "layout(xfb_offset = 0) out Geometry_data {\n";
        for (int x = 0; x < i; x++) {
            geometry += data[x] + ";\n";
        }
        geometry += "} geometry_data;\n";

        geometry += R"(
            void main() {
                for (int i = 0; i < gl_in.length(); i++) {
                    vec3 abs_position = abs(vertex_data[i].position.xyz);
                    if (abs_position.x < 1 && abs_position.y < 1) {
            )";

        iterator = this->m_result_buffer->get_layout().begin();
        while (iterator != this->m_result_buffer->get_layout().end()) {
            geometry += "geometry_data.instance_" + iterator->m_name + " = instance_output[i].instance_"+iterator->m_name + ";\n";
//            geometry += "geometry_data.instance_" + iterator->m_name + " = vec4(1,1,1,1);\n";
            iterator ++;
        }

        geometry += R"(
                EmitVertex();
                }
            }
            EndPrimitive();
            }
            )";
//        CORE_LOG_INFO(geometry);
        auto geometry_stage = std::make_shared<GL_shader_stage>(OPENGL_GEOMETRY_SHADER, std::move(geometry));

        this->shader_program = std::unique_ptr<GL_shader>(new GL_shader({vertex_stage, geometry_stage}));
        this->shader_program->compile();
        this->shader_program->bind();

        this->m_vao = std::make_unique<GL_vertex_array>();
        this->m_vao->add_vertex_buffer(this->m_vertices, 0);
        this->m_vao->add_vertex_buffer(this->m_vertex_data, 0);



        this->m_vao->bind();
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,this->m_result_buffer->get_ID());
        this->m_vao->unbind();
    }

    uint32_t &GL_Frustum_compute_query::fetch_result() {
        if (this->m_has_updated) {
            GLuint result;
                glGetQueryObjectuiv(m_last_frustum_query, GL_QUERY_RESULT,&result);
            this->set_result(result);
            this->m_has_updated = false;
        }
        return Frustum_compute_query::fetch_result();
    }

}