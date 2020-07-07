#include "Frustum_compute_query.h"
#include "Renderer_API.h"
#include <Core/Log.h>
#include <Platform/OpenGL/GL_Frustum_compute_query.h>

#include <Debug/Instrumentor.h>
namespace Cecilion {

    void Frustum_compute_query::execute() {
        /// Automatically resize the output buffer if the data size has changed.
        if (this->m_result_buffer->get_size() != this->m_vertex_data->get_size()) {
            this->m_result_buffer->reset_buffer(nullptr, this->m_vertex_data->get_size());
        }
    }

    uint32_t &Frustum_compute_query::fetch_result() {
        return this->m_last_result;
    }

    std::shared_ptr<Frustum_compute_query> Frustum_compute_query::Create(std::shared_ptr<Vertex_buffer> vertex_data_buffer, std::shared_ptr<Vertex_buffer> vertices) {
        switch(Render::Renderer_API::get_API()) {
            case Render::Renderer_API::API::OpenGL:
                return std::make_shared<GL_Frustum_compute_query>(vertex_data_buffer, vertices);
                break;
            case Render::Renderer_API::API::None:
                CORE_ASSERT(false, "Renderer::Frustum_compute_query:: Could not find a render API!");
                break;
        }
        return nullptr;
    }

    Frustum_compute_query::Frustum_compute_query(std::shared_ptr<Vertex_buffer> vertex_data_buffer, std::shared_ptr<Vertex_buffer> vertices) :
            m_vertex_data(vertex_data_buffer),
            m_last_result{0},
            m_vertices(vertices){

        this->m_result_buffer = std::shared_ptr<Vertex_buffer>(Cecilion::Vertex_buffer::Create(nullptr, vertex_data_buffer->get_size(), Vertex_buffer::Access_frequency::DYNAMIC, Vertex_buffer::Access_type::COPY));
        this->m_result_buffer->set_layout(vertex_data_buffer->get_layout());
    }


    void Frustum_compute_query::set_result(const uint32_t &instances) {
        this->m_last_result = instances;
    }

    std::shared_ptr<Vertex_buffer> Frustum_compute_query::get_result_buffer() {
        return this->m_result_buffer;
    }
}