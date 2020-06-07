#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <Core/Log.h>

#define OPENGL_VERTEX_SHADER (35633)
#define OPENGL_FRAGMENT_SHADER (35632)
// TODO these defines wont be needed if shaders are compiled from assets.
namespace Cecilion {



    class GL_shader {
    public:
        struct Shader_program {
            friend GL_shader;
        private:
            unsigned int program;
            Shader_program(unsigned int program) : program(program){}
        public:
            void bind();
            void unbind();
        };

        /**
         * Link one or more shaders into a shader program.
         * @tparam Args
         * @param shaders - This must be valid shaders of the type GLuint, or unsigned int.
         * @return
         */
        template<typename... Args>
        static Shader_program create_shader_program(Args... shaders) {
            auto program = initialize_shader_program();
            (attach_shader(program, shaders),...);
            auto result = link_shader_program(program);
            (detach_shader(program, shaders),...);
            return result;
        }

        static uint32_t create_shader(const uint32_t shader_type, const std::string& shader_source);
        /**
         * This effectively deletes the shader from memory. It can no longer be bound to programs.
         * @param shader
         */
        static void delete_shader(const uint32_t& shader);

    private:

        static unsigned int initialize_shader_program();
        static Shader_program link_shader_program(unsigned int program);

        static void attach_shader(unsigned int program, unsigned int shader);
        static void detach_shader(unsigned int program, unsigned int shader);
    public:

    };

}

