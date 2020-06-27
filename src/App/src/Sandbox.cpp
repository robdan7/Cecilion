#include <iostream>
#include <utility>
#include <bitset>
#include <vector>
#include <Utils/Sparse_set.h>

#define API_OPENGL
#include <Cecilion.h>
#include <Renderer/Shader_constants.h>
#include <tuple>
//#include <Core/Keycodes.h>

std::shared_ptr<Cecilion::Shader> create_shader() {
    auto vert = Cecilion::Shader::create_shader_stage(OPENGL_VERTEX_SHADER, R"(
            #version 450 core
            in vec4 in_position;
            in mat4 matrix;
            in vec4 instance_color;

            out vec4 vertex_color;
//            uniform Another_Uniform_block {
//                float scale;
//                vec4 color;
//                vec3 position;
//                mat4 matrix;
//            } another_block;

            uniform Uniform_block {
                mat4 matrix;
                float scale;
            } my_block;



            void main() {
                vertex_color = instance_color;
                vec4 position = in_position;
//                position.x += my_block.scale;
//                position.x += my_block.testz.z;
                gl_Position =my_block.matrix*matrix* position;
            }
            )");

    auto frag = Cecilion::Shader::create_shader_stage(OPENGL_FRAGMENT_SHADER, R"(
            #version 450 core
            in vec4 vertex_color;
            out vec4 color;

            void main() {
                color = vertex_color;
            }
            )");

    auto program =  Cecilion::Shader::create_shader({vert, frag});
    program->compile();

    // TODO This should be done automatically.
    /// Bind uniform block index to block binding 0.
    GLuint index= glGetUniformBlockIndex(program->get_ID(), "Uniform_block");
    glUniformBlockBinding(program->get_ID(), index,0);

    return program;
}

std::shared_ptr<Cecilion::Shader> create_shader(const char* name) {
    std::ifstream inputStream(name, std::ios::binary);
    std::istreambuf_iterator<char> start_it(inputStream), end_it;
    std::vector<char> buffer(start_it, end_it);
    inputStream.close();
    return Cecilion::Shader::load_binary_shader(36385, buffer.data(), buffer.size());
}

std::shared_ptr<Cecilion::Vertex_buffer> create_buffer() {
    float vertices[4 * (2)] = {
            -0.1f, -0.1f, //0.0f,1, //0, 0.3, 1, 1.0,
            0.0f, -0.1f, //0.0f,1,  //0.9, 1, 0.2, 1.0,
            0.0f, 0.0f, //0.0f,1,   //0.9, 0.3, 0.2, 1.0,
            -0.1f, 0.0f, //0.0f,1//,  0.9, 0.3, 0.2, 1.0
    };
    std::shared_ptr<Cecilion::Vertex_buffer> vertex_buffer =
            Cecilion::Vertex_buffer::Create(
                    vertices,
                    sizeof(vertices),
                    Cecilion::Vertex_buffer::Access_frequency::STATIC,
                    Cecilion::Vertex_buffer::Access_type::DRAW);

    Cecilion::Buffer_layout layout = {
            {Cecilion::Shader_data::Float2, "position"}
//            {Cecilion::Shader_data::Float4, "color"}
    };
    vertex_buffer->set_layout(layout);
    return vertex_buffer;
}

std::shared_ptr<Cecilion::Index_buffer> create_index_buffer() {
    uint32_t indices[6] = {0,2,3,0,1,2};
    std::shared_ptr<Cecilion::Index_buffer> index_buffer = std::shared_ptr<Cecilion::Index_buffer>(Cecilion::Index_buffer::Create(indices, 6));
    return index_buffer;
}

class Test : public Cecilion::Event_actor_st<Cecilion::Keyboard_key_Event, Cecilion::Mouse_cursor_Event> {
public:
    Test() : Cecilion::Event_actor_st<Cecilion::Keyboard_key_Event,Cecilion::Mouse_cursor_Event>() {
        this->set_callback<Cecilion::Mouse_cursor_Event>([](auto event){
            CORE_LOG_INFO("User moved the mouse!");
        });
        this->set_callback<Cecilion::Keyboard_key_Event>([](auto event){
            CORE_LOG_INFO("User pressed a key!");
        });
    }

};

/// Test layer for instanced rendering.
class Mesh_layer : public Cecilion::Layer<> {
public:
    ~Mesh_layer() override {
        CECILION_PROFILE_END_SESSION;
    }

    Mesh_layer(): filter(Cecilion::ECS::get_filter<>()){
        CECILION_PROFILE_BEGIN_SESSION("frustum culling", "frustum culling.json");

        this->m_mesh = std::shared_ptr<Cecilion::Instanced_mesh>(new Cecilion::Instanced_mesh({
            {Cecilion::Shader_data::Mat4, "matrix"},
            {Cecilion::Shader_data::Float4, "color"}}));
        this->m_mesh->add_LOD(create_buffer(), create_index_buffer(), create_shader());

        int i = 0;
        float size = 2;
        for (float x = -size; x < size; x += 0.15f) {
            for (float y = -size; y < size; y += 0.15f) {

                float matrices[16+4] = {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        x, y, 0,1,
                        0.0f,x,y,1
                };
                this->m_mesh->add_instance(matrices, glm::vec3{x, y, 0}, 1);
                i++;
            }
        }
        CORE_LOG_INFO("Computed {0} instances", i);
    }
    void on_update() override {

        Cecilion::Layer<>::on_update();
        this->m_mesh->on_update();
        Cecilion::Render_command::set_clear_color({0.2f,0.2f,0.2f,1.0f});
        Cecilion::Render_command::clear();
        Cecilion::Renderer::begin_scene();
        this->m_mesh->on_render();
        Cecilion::Renderer::end_scene();
    }

private:
    std::shared_ptr<Cecilion::Instanced_mesh> m_mesh;
    Cecilion::Filter<> filter;
};

class Input_layer : public Cecilion::Layer<Cecilion::Keyboard_key_Event, Cecilion::Mouse_cursor_Event> {
public:
    void on_update() override {
        this->check_inbox();
//        test->on_update();
//        Cecilion::Application::get().get_window().disable_cursor();
//        Layer::on_update();
//        this->camera.set_rotation(glm::quat(0,1,0,this->rotation));
//        this->camera.on_update();
//        Cecilion::params->CL_Viewport.m_data = this->camera.get_view_projection_matrix();
//        Cecilion::params->write((Cecilion::I_data<void*>*)&Cecilion::params->CL_Viewport);
    }
    Input_layer() : camera(0.01f, 10.0f, 45,1) {
//        this->test = new Test();
        this->camera.set_position(glm::vec3(0,0,6.0f));
        this->camera.look_at(glm::vec3(-1,0,0));
//        this->camera.set_rotation(glm::quat(0,1,0,17.0f));
//        this->camera.set_rotation(glm::quat(0,1,0,360.0f));
        this->camera.on_update();
        Cecilion::params->CL_Viewport.m_data = this->camera.get_view_projection_matrix();
        Cecilion::params->write((Cecilion::I_data<void*>*)&Cecilion::params->CL_Viewport);

        this->set_callback<Cecilion::Mouse_cursor_Event>([this](auto event){
            this->camera.set_rotation(event.xpos/1000,(event.ypos)/1000,0);
            this->camera.on_update();
            Cecilion::params->CL_Viewport.m_data = this->camera.get_view_projection_matrix();
            Cecilion::params->write((Cecilion::I_data<void*>*)&Cecilion::params->CL_Viewport);
        });
        this->set_callback<Cecilion::Keyboard_key_Event>([this](auto event){
            LOG_INFO("User pressed key! Key state: {0}", event.action);
            if (event.keycode == CL_KEY_DELETE) {
                if (event.action) {
                    if (!this->state) {
                        Cecilion::Application::get().get_window().disable_cursor();
                    } else {
                        Cecilion::Application::get().get_window().show_cursor();
                    }
                    this->state = !this->state;
                }

            }
//
        });

    }
private:
    bool state = false;
    float rotation = 0.0f;
    Cecilion::Perspective_camera camera;
//    Test* test;
};



//Cecilion::CL_System_params params;

class App : public Cecilion::Application {
public :
    App() {
//        this->camera.set_position(glm::vec3(0,0,3.0f));
//        this->camera.look_at(glm::vec3(0,0,-1));
//        this->camera.set_rotation(glm::quat(0,1,0,0.0f));
//        this->camera.on_update();

        Cecilion::init_shader_constants();
//        Cecilion::params->CL_Time.m_data = 2;
//        Cecilion::params->CL_Viewport.m_data = this->camera.get_view_projection_matrix();

//        Cecilion::params->CL_Test.m_data = glm::vec3(0.5f,0,0);
//        Cecilion::params->write((Cecilion::I_data<void*>*)&Cecilion::params->CL_Test);
//        Cecilion::params->write((Cecilion::I_data<void*>*)&Cecilion::params->CL_Time);
//        Cecilion::params->write((Cecilion::I_data<void*>*)&Cecilion::params->CL_Viewport);

//        Cecilion::params->m_buffer->dump_to_file("Constant_buffer.bin");
//        Data_container c = Data_container();
//        c.scale.my_float = 1;
//        c.write((I_data*)&c.scale);


//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Example_layer<>())));
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Mesh_layer())));
//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Actor_layer())));
        Cecilion::Layer<>* imgui = new Cecilion::ImGui_layer();
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(imgui)));

        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<Cecilion::Keyboard_key_Event, Cecilion::Mouse_cursor_Event>>(new Input_layer())));
    }

    ~App() override {

    }
private:


};

Cecilion::Application* Cecilion::CreateApplication() {

//    auto lambda = []<int>(){
//
//    };
//
//    auto tup = new std::tuple<int, std::string>();
//
//    std::get<int>(*tup) = 2;
//
//    LOG_INFO("My number is {0}", std::get<int>(*tup));
//    LOG_INFO("My number is {0}", std::get<int>(*(std::tuple<int,std::string>*)tup));

    return new App();
}