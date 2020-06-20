#include "Distance_compute_query.h"

Cecilion::Distance_compute_query::Distance_compute_query(std::shared_ptr<Vertex_buffer> dataBuffer,
                                                         std::shared_ptr<Vertex_buffer> points)
        : Frustum_compute_query(dataBuffer, points) {

}

Cecilion::Distance_compute_query::~Distance_compute_query() {

}

void Cecilion::Distance_compute_query::execute() {
    Frustum_compute_query::execute();
}

uint32_t &Cecilion::Distance_compute_query::fetch_result() {
    return Frustum_compute_query::fetch_result();
}
