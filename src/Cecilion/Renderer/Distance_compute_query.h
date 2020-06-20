#pragma once
#include "Frustum_compute_query.h"
namespace Cecilion {
    class Distance_compute_query : public Frustum_compute_query {
    public:
        ~Distance_compute_query() override;
        void execute() override;

        uint32_t &fetch_result() override;
    protected:
        Distance_compute_query(std::shared_ptr<Vertex_buffer> dataBuffer, std::shared_ptr<Vertex_buffer> points);
    };
}



