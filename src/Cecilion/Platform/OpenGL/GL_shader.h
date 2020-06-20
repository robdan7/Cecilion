#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <Core/Log.h>
#include <iostream>
#include <Renderer/Shader.h>
#include <initializer_list>

/// TODO These defines should not be here.
#define OPENGL_VERTEX_SHADER (35633)
#define OPENGL_FRAGMENT_SHADER (35632)
#define OPENGL_GEOMETRY_SHADER (0x8DD9)

namespace Cecilion {
    class GL_shader;
    class GL_shader_stage : public Cecilion::Shader_stage {
        friend class GL_shader;
    public:
        GL_shader_stage(const uint32_t shader_type, const std::string&& shader_source);


        ~GL_shader_stage() override;
    private:
        void delete_shader(const uint32_t& shader);
        uint32_t shader;
    };
    class GL_shader : public Cecilion::Shader {
    public:
        GL_shader(std::initializer_list<std::shared_ptr<Shader_stage>> shaderstages);

        /**
         * Load a precompiled shader. WARNING: The binary format is vendor specific, which means
         * one binary shader won't load on a different gpu. the specific binary format can be
         * retrieved with glGetProgramBinary. The specific format for my gpu is 36385.
         * @param binary_format
         * @param binary
         * @param size
         */
        GL_shader(uint32_t binary_format, const char *binary, uint32_t size);
        ~GL_shader() override;
        void bind() override;
        void unbind() override;

        void compile() override;

        void attach_shader_stage(std::shared_ptr<Shader_stage> shader_stage) override;

        uint32_t get_ID() override {
            return this->m_program;
        }

    private:
        uint32_t m_program;
        int m_linked;


    };

}

