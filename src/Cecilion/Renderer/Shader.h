#pragma once
#include <initializer_list>
#include <cstdint>
#include <string>
#include "Buffer.h"
namespace Cecilion {
    /**
     * Generic shader class.
     */
    class Shader_stage {
    public:
        virtual ~Shader_stage() {};
    };
    class Shader {
    public:
        virtual ~Shader() {};

        static std::shared_ptr<Shader_stage> create_shader_stage(const uint32_t shader_type, const std::string&& shader_source);
        static std::shared_ptr<Shader> load_binary_shader(uint32_t binary_format, const char *binary, uint32_t size);

        static std::shared_ptr<Shader> create_shader(const std::initializer_list<std::shared_ptr<Shader_stage>> elements);
        virtual void attach_shader_stage(std::shared_ptr<Shader_stage> shader_stage) = 0;
        virtual void compile() = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        // TODO remove this. Its API specific.
        virtual uint32_t get_ID() = 0;
    };
}
