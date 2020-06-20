#pragma once
#include <Renderer/Frustum_compute_query.h>
namespace Cecilion {
    class GL_shader;
    class GL_vertex_array;
    class GL_Frustum_compute_query : public Frustum_compute_query {
    public:
        GL_Frustum_compute_query(std::shared_ptr<Vertex_buffer> data_buffer, std::shared_ptr<Vertex_buffer> instance_data);

        ~GL_Frustum_compute_query() override;

        void execute() override;

        /**
         * Fetch the result from OpenGL. This will stall the cpu if the query has not been processed
         * by the gpu.
         *
         * To ensure that the result can be retrieved, execute the query as soon as possible every frame.
         * Executing it directly before using the result will inevitably cause lag.
         * @return - The number of vertices that passed the frustum culling test. The resulting data buffer will
         * automatically be written to; there is no need to call this function if you don't need the
         * number of vertices.
         */
        uint32_t &fetch_result() override;

    private:
        std::unique_ptr<GL_shader> shader_program;
        std::unique_ptr<GL_vertex_array> m_vao;
        uint32_t m_last_frustum_query;
        bool m_has_updated = false;
    };
}
