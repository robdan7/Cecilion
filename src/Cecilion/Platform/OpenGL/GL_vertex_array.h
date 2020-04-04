#pragma once

#include <Renderer/Vertex_array.h>
namespace Cecilion {
    class GL_vertex_array : public Vertex_array {
    public:
        GL_vertex_array();
        ~GL_vertex_array();
        void bind() const override;

        void unbind() const override;

        void add_vertex_buffer(const std::shared_ptr<Vertex_buffer>& vertex_buffer) override;

        void set_index_buffer(const std::shared_ptr<Index_buffer>& index_buffer) override;

        const std::vector<std::shared_ptr<Vertex_buffer>> &get_vertex_buffer() const override;

        const std::shared_ptr<Index_buffer> &get_index_buffer() const override;

    private:
        uint32_t m_render_ID;
        std::vector<std::shared_ptr<Vertex_buffer>> m_vertex_buffers;
        std::shared_ptr<Index_buffer> m_index_buffer;
    };
}


