#include "GL_Frustum_compute_query.h"
#include <Platform/OpenGL/GL_shader.h>
#include <Platform/OpenGL/GL_vertex_array.h>
#include <Platform/OpenGL/OpenGL.h>
#include <Debug/Instrumentor.h>
#include <iostream>
#include <fstream>

namespace Cecilion {

    GL_Frustum_compute_query::~GL_Frustum_compute_query() {

    }

    void GL_Frustum_compute_query::execute() {
        Frustum_compute_query::execute();
        this->shader_program->bind();
        this->m_vao->bind();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->m_TFO);

        glEnable(GL_RASTERIZER_DISCARD);
        glBeginTransformFeedback(GL_POINTS);
//        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_last_frustum_query);
        glBeginQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,1, m_last_frustum_query);
        glDrawArrays(GL_POINTS, 0, this->m_vertices->get_size() / this->m_vertices->get_layout().get_stride());
//        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
        glEndQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,1);
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);

        glFlush();  // Make sure all commands are transported into the pipeline.
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        this->m_vao->unbind();
        this->shader_program->unbind();
        this->m_has_updated = true;
    }

    GL_Frustum_compute_query::GL_Frustum_compute_query(std::shared_ptr<Vertex_buffer> data_buffer, std::shared_ptr<Vertex_buffer> instance_data)
            : Frustum_compute_query(data_buffer, instance_data) {

        // TODO do some actual frustum culling.
        // TODO Code cleanup. String concatenation results in unnecessary temporary strings.

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

        auto vertex_stage = std::make_shared<GL_shader_stage>(OPENGL_VERTEX_SHADER, std::move(vertex_shader));

        /// -------- Geometry shader ---------
        std::string geometry = R"(
            #version 440 core
            layout (invocations = 2, points) in;
            layout (points, max_vertices = 10) out;

            in Vertex_data {
                vec4 position;
//                float radius;
            } vertex_data[];

//            layout(stream = 1) out vec4 stream_position;

        )";

        geometry += "in Instance_data {\n";
        for (int x = 0; x < i; x++) {
            geometry += data[x] + ";\n";
        }
        geometry += "} instance_output[];\n";

        /// Create one output buffer for each stream/LOD.
        for(int buffers = 0; buffers < 4; buffers++) {
            std::string stream = std::to_string(buffers);
            geometry += "layout(stream = "+stream+", xfb_buffer = "+stream+", xfb_offset=0) out Geometry_data_stream_"+stream+" {\n";
            for (int x = 0; x < i; x++) {
                geometry += data[x] + ";\n";
            }
            geometry += "} geometry_data_stream_"+std::to_string(buffers)+";\n";
        }

        geometry += R"(
            void main() {
//                for (int i = 0; i < gl_in.length(); i++) {
//                    vec3 abs_position = abs(vertex_data[i].position.xyz);
//                    if (vertex_data[i].position.x < 0.5f) {
            )";

        std::string stream = std::to_string(1);
        iterator = this->m_result_buffer->get_layout().begin();
        while (iterator != this->m_result_buffer->get_layout().end()) {
            geometry += "geometry_data_stream_"+stream+".instance_" + iterator->m_name + " = instance_output[0].instance_"+iterator->m_name + ";\n";
            geometry += "geometry_data_stream_1.instance_" + iterator->m_name + " = instance_output[0].instance_"+iterator->m_name + ";\n";
            iterator ++;
        }

        geometry += R"(
                EmitStreamVertex(gl_InvocationID);
                EndStreamPrimitive(gl_InvocationID);
            }
            )";

        std::ofstream myfile;
        myfile.open("geometry_shader.txt");
        myfile << geometry;
        myfile.close();

        auto geometry_stage = std::make_shared<GL_shader_stage>(OPENGL_GEOMETRY_SHADER, std::move(geometry));

        this->shader_program = std::unique_ptr<GL_shader>(new GL_shader({vertex_stage, geometry_stage}));
        this->shader_program->compile();
        this->shader_program->bind();

        this->m_vao = std::make_unique<GL_vertex_array>();
        this->m_vao->add_vertex_buffer(this->m_vertices, 0);
        this->m_vao->add_vertex_buffer(this->m_vertex_data, 0);

        glGenTransformFeedbacks(1, &this->m_TFO);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->m_TFO);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,1,this->m_result_buffer->get_ID());
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        glGenQueries(1,&m_last_frustum_query);
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