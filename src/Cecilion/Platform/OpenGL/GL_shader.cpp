#include <string>
#include "GL_shader.h"
#include <Core/Log.h>
#include "OpenGL.h"

namespace Cecilion {


    GL_shader::GL_shader() {
        this->m_renderID = glCreateProgram();
    }




    GL_shader::~GL_shader() {
        glDeleteProgram(this->m_renderID);
    }


    void GL_shader::bind() {
        glUseProgram(this->m_renderID);
    }

    void GL_shader::unbind() {
        glUseProgram(0);
    }

    /**
     * Attach a shader to this program. The shader itself will not be attached until
     * the program is linked, which means that any shader can be unwillingly deleted until it
     * is attached.
     * @param shader
     */
    void GL_shader::attach_shader(const uint32_t& shader) {
        CORE_ASSERT(!this->linked, "Cannot attach shader to linked shader program!");
        this->attached_shaders.push_back(shader);
    }

    void GL_shader::link() {
        for (uint32_t shader: this->attached_shaders) {
            glAttachShader(this->m_renderID, shader);
        }
        glLinkProgram(m_renderID);

                GLint isLinked = 0;
        glGetProgramiv(m_renderID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_renderID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_renderID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_renderID);
            // Don't leak shaders either.
            //glDeleteShader(vertexShader);
            //glDeleteShader(fragmentShader);

            CORE_LOG_ERROR("Shader program compilation failed! \n    Info: {0}", infoLog.data());
            return;
        }

        for (uint32_t shader: this->attached_shaders) {
            glDetachShader(this->m_renderID, shader);
        }
        this->attached_shaders.clear();
        this->linked = true;
    }

    uint32_t GL_shader::create_shader(const uint32_t shader_type, const std::string &shader_source) {
        /// Based on the school example from
        /// https://www.khronos.org/opengl/wiki/Shader_Compilation
        // Create an empty vertex shader handle
        uint32_t shader_ID = glCreateShader(shader_type);

        const GLchar *source = (const GLchar *)shader_source.c_str();
        glShaderSource(shader_ID, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(shader_ID);

        GLint isCompiled = 0;
        glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
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

    void GL_shader::delete_shader(const uint32_t& shader) {
        glDeleteShader(shader);
    }


}

