#include "Instanced_mesh.h"
#include <Renderer/Renderer.h>
#include <Debug/Instrumentor.h>
namespace Cecilion {

    Instanced_mesh::Instanced_mesh(const std::initializer_list<Buffer_element> &instance_layout) {
        this->m_instance_buffer_data = std::shared_ptr<Vertex_buffer>(Vertex_buffer::Create(nullptr, 0, Vertex_buffer::Access_frequency::DYNAMIC, Vertex_buffer::Access_type::DRAW));
        this->m_instance_buffer_data->set_layout(instance_layout);

        this->m_instance_bounding_spheres = std::shared_ptr<Vertex_buffer>(Vertex_buffer::Create(nullptr,0, Vertex_buffer::Access_frequency::DYNAMIC, Vertex_buffer::Access_type::DRAW));
        this->m_instance_bounding_spheres->set_layout(Buffer_layout{{Shader_data::Float4, "position"}});

        this->m_frustum_pipeline = Frustum_compute_query::Create(m_instance_buffer_data, m_instance_bounding_spheres);
    }

    void
    Instanced_mesh::add_LOD(std::shared_ptr<Vertex_buffer> vertex_data, std::shared_ptr<Index_buffer> index_buffer,
                            std::shared_ptr<Shader> shader) {
        this->m_lod.emplace_back(Mesh_data{vertex_data, index_buffer}, shader, this->m_frustum_pipeline);

    }

    void Instanced_mesh::add_instance(float *data, glm::vec3 position, float radius) {
        auto instance_data_size = this->m_instance_buffer_data->get_layout().get_stride();
        this->m_instance_buffer_data->resize_buffer(this->m_instance_buffer_data->get_size()+instance_data_size);
        this->m_instance_bounding_spheres->resize_buffer(this->m_instance_bounding_spheres->get_size() + this->m_instance_bounding_spheres->get_layout().get_stride());

        this->m_instance_buffer_data->set_sub_data(data, this->instances*instance_data_size, instance_data_size);
        float sphere[] = {position.x, position.y, position.z, radius};
        this->m_instance_bounding_spheres->set_sub_data(sphere, this->instances*m_instance_bounding_spheres->get_layout().get_stride(), m_instance_bounding_spheres->get_layout().get_stride());
        this->instances ++;
    }

    void Instanced_mesh::on_render() {
        for (auto lod : this->m_lod) {
            lod.on_render();
        }
    }

    void Instanced_mesh::on_update() {
        this->m_frustum_pipeline->execute();
    }

    Instanced_mesh::Mesh_LOD::Mesh_LOD(Mesh_data mesh, std::shared_ptr<Shader> shader,
                                       std::shared_ptr<Frustum_compute_query> frustum_pipeline) : m_frustum_pipeline(frustum_pipeline) {
        this->m_vao = std::shared_ptr<Cecilion::Vertex_array>(Cecilion::Vertex_array::Create());
        this->m_vao->add_vertex_buffer(mesh.p_buffer,0);

        this->m_vao->add_vertex_buffer(frustum_pipeline->get_result_buffer(),1);
        this->m_vao->set_index_buffer(mesh.p_index_buffer);

        this->m_shader = shader;
    }

    void Instanced_mesh::Mesh_LOD::on_render() {
        this->m_shader->bind();
        {
            CECILION_PROFILE_FUNCTION();
            Cecilion::Renderer::submit_instanced(this->m_vao, this->m_frustum_pipeline->fetch_result());
        }
        this->m_shader->unbind();
    }
}


