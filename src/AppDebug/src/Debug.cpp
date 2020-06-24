#include <iostream>
#include <utility>
#include <bitset>
#include <vector>
#include <typeinfo>
#include <Utils/Sparse_set.h>
#include <functional>

#define API_OPENGL
#include <Cecilion.h>
#include <Platform/OpenGL/GL_Frustum_compute_query.h>

#include <thread>

void frustum_culling() {
    auto vertex = Cecilion::Shader::create_shader_stage(GL_VERTEX_SHADER, R"(
#version 440 core
void main() {
}
    )");
    auto geometry = Cecilion::Shader::create_shader_stage(GL_GEOMETRY_SHADER, R"(
#version 440 core
layout(points) in;
layout(points, max_vertices = 1) out;
layout(stream = 0, xfb_buffer=0, xfb_offset=0) out Baz {
    int bar;
    int foo[3];
} foobar;

void main() {
    foobar.foo = int[3](0x41, 0x42, 0x43);
    foobar.bar = 0x44;
    EmitStreamVertex(0);
    EndStreamPrimitive(0);
}
    )");

    auto shader = Cecilion::Shader::create_shader({vertex, geometry});
//    const char *varyings[] = {"foo"};
//    glTransformFeedbackVaryings(shader->get_ID(), 1, varyings, GL_INTERLEAVED_ATTRIBS);
    shader->compile();
    shader->bind();

    auto vbo1 = std::shared_ptr<Cecilion::Vertex_buffer>(Cecilion::Vertex_buffer::Create(nullptr, 160, Cecilion::Vertex_buffer::Access_frequency::DYNAMIC, Cecilion::Vertex_buffer::Access_type::COPY));

    uint32_t TFO;
    glGenTransformFeedbacks(1, &TFO);

    uint32_t query;
    glGenQueries(1,&query);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFO);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo1->get_ID());


    glEnable(GL_RASTERIZER_DISCARD);
    glBeginTransformFeedback(GL_POINTS);
    glBeginQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,0, query);
    glDrawArrays(GL_POINTS, 0, 1);
    glEndQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,0);
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_COPY_READ_BUFFER, reinterpret_cast<GLuint>(vbo1->get_ID()));
    char data[vbo1->get_size()];
    glGetBufferSubData(GL_COPY_READ_BUFFER, 0, vbo1->get_size(), &data);

    /// Get transform feedback primitives.
    GLuint result;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT,&result);
    LOG_INFO("Transform feedback gave: {0}", result);

    std::ofstream myfile;
    myfile.open("data.bin");
    myfile.write(data, vbo1->get_size());
    myfile.close();
}

class App : public Cecilion::Application {
public :
    App() {
        frustum_culling();
        Cecilion::Event_system::post<Cecilion::Window_close_event>();
    }

    ~App() {

    }

};

Cecilion::Application* Cecilion::CreateApplication() {



    return new App();
}