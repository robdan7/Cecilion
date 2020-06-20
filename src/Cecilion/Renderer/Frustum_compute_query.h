#pragma once
#include "Compute_query.h"
#include <cstdint>
#include <Renderer/Buffer.h>
namespace Cecilion {
    // TODO Add support for points, circles and bounding boxes.
    class Frustum_compute_query : public Compute_query<uint32_t, std::shared_ptr<Vertex_buffer>>{
    public:
        virtual inline ~Frustum_compute_query() {}
        void execute() override;
        virtual uint32_t &fetch_result() override;

         /**
          *
          * @param vertex_data_buffer - A buffer filled with additional data for each vertex. Make sure this buffer is big enough.
          * @param vertices - The buffer filled with vec4 vertices. The w component is processed as the radius of a bounding sphere.
          * @return - A shared pointer to a generic frustum query.
          */
        static std::shared_ptr<Frustum_compute_query> Create(std::shared_ptr<Vertex_buffer> vertex_data_buffer, std::shared_ptr<Vertex_buffer> vertices);

        /**
         * @return - Retrieve the data buffer produced by the query. This will be equal to the input data buffer if all
         * vertices pass the culling test.
         */
        std::shared_ptr<Vertex_buffer> get_result_buffer();
    protected:
        Frustum_compute_query(std::shared_ptr<Vertex_buffer> vertex_data_buffer, std::shared_ptr<Vertex_buffer> vertices);
        void set_result(const uint32_t &instances);

    protected:
        std::shared_ptr<Vertex_buffer> m_result_buffer;
        std::shared_ptr<Vertex_buffer> m_vertex_data;
        std::shared_ptr<Vertex_buffer> m_vertices;
    private:
        uint32_t m_last_result;
    };
}
