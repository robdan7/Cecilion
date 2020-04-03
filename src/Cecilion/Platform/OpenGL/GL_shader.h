#pragma once
namespace Cecilion {
    class GL_shader;

    class GL_shader {
    public:
        GL_shader();
        ~GL_shader();
        void attach_shader(const uint32_t& shader);
        void link();
        void bind();
        void unbind();
        static uint32_t create_shader(const uint32_t shader_type, const std::string& shader_source);
        static void delete_shader(const uint32_t& shader);
    private:
        std::vector<uint32_t> attached_shaders;
        bool linked;
        uint32_t m_renderID;
    };
}

