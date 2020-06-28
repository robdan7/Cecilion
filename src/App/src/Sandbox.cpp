#include <iostream>
#include <utility>
#include <bitset>
#include <vector>
#include <Utils/Sparse_set.h>

#define API_OPENGL
#include <Cecilion.h>


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
                mat4 view_matrix;
                mat4 projection_matrix;
                vec3 CL_Center_reference;
                float scale;
            } my_block;



            void main() {
                vec4 position = my_block.projection_matrix * my_block.view_matrix*matrix*in_position;
                vertex_color = instance_color;

//                position.x += my_block.scale;
//                position.x += my_block.testz.z;
                gl_Position =position;
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
    float vertices[4 * (4)] = {
            -0.12f, -0.12f, 0.0f,1, //0, 0.3, 1, 1.0,
            0.12f, -0.12f, 0.0f,1,  //0.9, 1, 0.2, 1.0,
            0.12f, 0.12f, 0.0f,1,   //0.9, 0.3, 0.2, 1.0,
            -0.12f, 0.12f, 0.0f,1//,  0.9, 0.3, 0.2, 1.0
    };
    std::shared_ptr<Cecilion::Vertex_buffer> vertex_buffer =
            Cecilion::Vertex_buffer::Create(
                    vertices,
                    sizeof(vertices),
                    Cecilion::Vertex_buffer::Access_frequency::STATIC,
                    Cecilion::Vertex_buffer::Access_type::DRAW);

    Cecilion::Buffer_layout layout = {
            {Cecilion::Shader_data::Float4, "position"}
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
Cecilion::Perspective_camera camera(0.001f, 100.0f, 45,1);
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
        float size = 5;
        for (float x = -size; x < size; x += 0.3f) {
            for (float y = -size; y < size; y += 0.3f) {

                float matrices[16+4] = {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        x, y, 0,1,
                        0.0f,x/5.0f,y/5.0f,1
                };
                this->m_mesh->add_instance(matrices, glm::vec3{x, y, 0}, sqrt(2)*0.12f);
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
        Cecilion::Renderer::begin_scene(camera);
        this->m_mesh->on_render();
        Cecilion::Renderer::end_scene();
    }

private:
    std::shared_ptr<Cecilion::Instanced_mesh> m_mesh;
    Cecilion::Filter<> filter;
};

class Input_layer : public Cecilion::Layer<Cecilion::Window_resize_event, Cecilion::Keyboard_key_Event, Cecilion::Mouse_cursor_Event> {
public:
    void on_update() override {
        this->check_inbox();
    }
    Input_layer() {
        camera.set_position_axis_aligned(glm::vec3(0, 0, 10.0f));
        camera.look_at(glm::vec3(-1,0,0));
        camera.on_update();

        this->set_callback<Cecilion::Mouse_cursor_Event>([this](auto event){
            camera.set_rotation(event.xpos/1000,(event.ypos)/1000,0);
            camera.on_update();
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
        });
        this->set_callback<Cecilion::Window_resize_event>([](Cecilion::Window_resize_event event){
            camera.set_aspect_ratio(event.width / event.height);
        });
    }
private:
    bool state = false;
    float rotation = 0.0f;
};


class App : public Cecilion::Application {
public :
    App() {
//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Example_layer<>())));
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<Cecilion::Window_resize_event, Cecilion::Keyboard_key_Event, Cecilion::Mouse_cursor_Event>>(new Input_layer())));
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Mesh_layer())));
//        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(new Actor_layer())));
        Cecilion::Layer<>* imgui = new Cecilion::ImGui_layer();
        this->append_layer(std::move(std::unique_ptr<Cecilion::Layer<>>(imgui)));
    }

    ~App() override {

    }
private:


};

Cecilion::Application* Cecilion::CreateApplication() {
    return new App();
}