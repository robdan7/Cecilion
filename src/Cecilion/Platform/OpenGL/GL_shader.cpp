#include <string>
#include "GL_shader.h"
#include <Core/Log.h>
#include "OpenGL.h"
#include <stdarg.h>

namespace Cecilion {
    uint32_t GL_shader::create_shader(const uint32_t shader_type, const std::string &shader_source) {
        /// Based on the school example from
        /// https://www.khronos.org/opengl/wiki/Shader_Compilation
        // Create an empty vertex shader handle
        uint32_t shader_ID = glCreateShader(shader_type);

        const GLchar *source = (const GLchar *) shader_source.c_str();
        glShaderSource(shader_ID, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(shader_ID);

        GLint isCompiled = 0;
        glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader_ID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(shader_ID);

            CORE_LOG_ERROR("{0} shader compilation failed! \n    Info: {1}", shader_type, infoLog.data());
            return -1;
        }
        return shader_ID;
    }

    void GL_shader::delete_shader(const uint32_t &shader) {
        glDeleteShader(shader);
    }


    GL_shader::Shader_program GL_shader::link_shader_program(unsigned int program) {
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *) &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            //glDeleteShader(vertexShader);
            //glDeleteShader(fragmentShader);

            // TODO Fix proper error callback. The previous version didn't work.
            CORE_LOG_ERROR("Shader program {0} compilation failed! \n", program);
//            CORE_LOG_ERROR("Info:    {0}", infoLog.data());
            return Shader_program{0};
        }
//        CORE_LOG_INFO("linked shader {0}", this->m_renderID);
        return Shader_program{program};
    }



        unsigned int GL_shader::initialize_shader_program() {
            return glCreateProgram();
        }

        void GL_shader::attach_shader(unsigned int program, unsigned int shader) {
            glAttachShader(program, shader);
        }

        void GL_shader::detach_shader(unsigned int program, unsigned int shader) {
            glDetachShader(program, shader);
        }



    void GL_shader::Shader_program::bind() {
        glUseProgram(this->program);
    }

    void GL_shader::Shader_program::unbind() {
        glUseProgram(0);
    }

}

