#pragma once

#include "Buffer.h"
namespace Cecilion {
    /**
     * Just a generic vertex array class.
     */
    class Vertex_array {
    public:
        virtual ~Vertex_array();
        virtual void bind() const =0;
        virtual void unbind() const = 0;
        virtual void add_vertex_buffer(const std::shared_ptr<Vertex_buffer>& vertex_buffer, uint32_t instance_divisor) = 0;
        virtual void set_index_buffer(const std::shared_ptr<Index_buffer>& index_buffer) = 0;
        static Vertex_array* Create();
        virtual const std::vector<std::shared_ptr<Vertex_buffer>>& get_vertex_buffer() const = 0;
        virtual const std::shared_ptr<Index_buffer>& get_index_buffer() const = 0;
    };
}


