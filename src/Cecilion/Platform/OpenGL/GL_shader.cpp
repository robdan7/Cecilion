#include <string>
#include "GL_shader.h"
#include "OpenGL.h"
#include <Core/Log.h>
#include <stdarg.h>
#include <fstream>
#include <iterator>
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
namespace Cecilion {
    GL_shader::GL_shader(std::initializer_list<std::shared_ptr<Shader_stage>> shaderstages) {
        this->m_program = glCreateProgram();
        for (std::shared_ptr<Shader_stage> stage : shaderstages) {
            glAttachShader(m_program, static_cast<GL_shader_stage*>(stage.get())->shader);
        }

    }


    void GL_shader::GL_shader::bind() {
        glUseProgram(this->m_program);
    }

    void GL_shader::GL_shader::unbind() {
        glUseProgram(0);
    }

    GL_shader::~GL_shader() {
//        Shader::~Shader();
        glDeleteProgram(m_program);
    }

    void GL_shader::compile() {
        if (this->m_linked == GL_TRUE) {
            CORE_LOG_ERROR("GL_shader:: Tried to link program ID {0} twice", this->m_program);
            return;
        }
//        glProgramParameteri(this->m_program,GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE); // TODO remove
        glLinkProgram(m_program);

        /// Check if the linking worked.
        glGetProgramiv(m_program, GL_LINK_STATUS, (int *) &m_linked);
        if (m_linked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_program);

            // TODO Fix proper error callback. The previous version didn't work.
//            CORE_LOG_ERROR("Shader program {0} compilation failed! \n", m_program);
            std::ostringstream out;
            for (GLchar in : infoLog) {
                out << in;
            }
            std::string s(out.str());
            CORE_LOG_ERROR("Platform::GL_shader:: Could not link shader program. Error code:\n{0}", s);
        }
//        } else {
//
//
//            GLint length = 0;
//            glGetProgramiv(this->m_program, GL_PROGRAM_BINARY_LENGTH, &length);
//            char data[length];
//            GLenum format = 0;
//            glGetProgramBinary(this->m_program, length, NULL, &format, data);
////            CORE_LOG_INFO("Binary format: {0}", format);
//
//
//            std::ofstream out("shader_" + std::to_string(this->m_program) + ".bin", std::ios::binary);
//            out.write(data, length);
//            out.close();
//            CORE_LOG_INFO("Writing shader {0} to file, bytes: {1}, {2}", "shader_" + std::to_string(this->m_program) + ".bin", length, sizeof(data));
////
//            std::ifstream inputStream("shader_" + std::to_string(this->m_program) + ".bin", std::ios::binary);
//////            std::ifstream input_stream(fName, std::ios::binary);
//            std::istreambuf_iterator<char> start_it(inputStream), end_it;
//            std::vector<char> buffer(start_it, end_it);
//        inputStream.close();
//
//
////            CORE_LOG_INFO("Wrote {0} bytes to char vector", buffer.size());
//            glProgramBinary(this->m_program, format, buffer.data(), buffer.size() );
////            GL_shader(buffer.data());
////            test(this->m_program, buffer);
//
//        }
    }

    void GL_shader::attach_shader_stage(std::shared_ptr<Shader_stage> shader_stage) {
        glAttachShader(m_program, static_cast<GL_shader_stage*>(shader_stage.get())->shader);
    }

    GL_shader::GL_shader(uint32_t binary_format, const char *binary, uint32_t size) {
        this->m_program = glCreateProgram();
        glProgramBinary(this->m_program, binary_format, binary, size);

        /// Check if the linking worked.
        glGetProgramiv(m_program, GL_LINK_STATUS, (int *) &m_linked);
        if (m_linked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_program);

            // TODO Fix proper error callback. The previous version didn't work.
            CORE_LOG_ERROR("Shader program {0} compilation failed! \n", m_program);
        }
    }


    GL_shader_stage::GL_shader_stage(uint32_t shader_type, const std::string &&shader_source) {
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
            this->shader = 0;
        }
        this->shader = shader_ID;
    }

    GL_shader_stage::~GL_shader_stage() {
//        Shader_stage::~Shader_stage();
        glDeleteShader(shader);
    }
}

