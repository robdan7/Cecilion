#include "Instanced_mesh.h"
#include <Renderer/Renderer.h>
#include <Debug/Instrumentor.h>
#include "FBX_mesh.h"
namespace Cecilion {

    Instanced_mesh::Instanced_mesh(const std::initializer_list<Buffer_element> &instance_layout) {
        this->m_instance_buffer_data = std::shared_ptr<Vertex_buffer>(Vertex_buffer::Create(nullptr, 0, Vertex_buffer::Access_frequency::DYNAMIC, Vertex_buffer::Access_type::DRAW));
        this->m_instance_buffer_data->set_layout(instance_layout);

        this->m_instance_bounding_spheres = std::shared_ptr<Vertex_buffer>(Vertex_buffer::Create(nullptr,0, Vertex_buffer::Access_frequency::DYNAMIC, Vertex_buffer::Access_type::DRAW));
        this->m_instance_bounding_spheres->set_layout(Buffer_layout{{Shader_data::Float4, "position"}});

        this->m_frustum_pipeline = Frustum_compute_query::Create(m_instance_buffer_data, m_instance_bounding_spheres);
    }

    void Instanced_mesh::add_LOD(const char *file, const std::shared_ptr<Shader>& shader) {
        FBX_mesh* mesh = FBX_mesh::Create(file);
        this->m_lod.emplace_back(mesh->m_vertex_buffer);

        Cecilion::Buffer_layout layout;
        if (mesh->draw_type == Draw_type::STREAM) {
            layout = {
                    {Cecilion::Shader_data::Float3, "position"},
                    {Cecilion::Shader_data::Float3, "normal"}
            };
        } else {
            layout = {
                    {Cecilion::Shader_data::Float3, "position"}
            };
        }
        this->m_lod.back().p_buffer->set_layout(layout);


        for (auto& material : mesh->materials) {
            if (mesh->draw_type == Draw_type::STREAM) {
                this->m_lod.back().m_materials.emplace_back(Cecilion::Vertex_array::Create(),shader,material.starting_vertex,material.vertices);
            } else {
                this->m_lod.back().m_materials.emplace_back(Cecilion::Vertex_array::Create(),shader);
            }
            this->m_lod.back().m_materials.back().m_vao->add_vertex_buffer(this->m_lod.back().p_buffer,0);
            this->m_lod.back().m_materials.back().m_vao->add_vertex_buffer(this->m_frustum_pipeline->get_result_buffer(),1);
            this->m_lod.back().m_materials.back().m_vao->set_index_buffer(Cecilion::Index_buffer::Create(
                        reinterpret_cast<uint32_t *>(material.vertex_index.data()), material.vertex_index.size()));
        }
        delete mesh;
    }

    void Instanced_mesh::add_instance(float *data, glm::vec3 position, float radius) {
        auto instance_data_size = this->m_instance_buffer_data->get_layout().get_stride();
        this->m_instance_buffer_data->resize_buffer(this->m_instance_buffer_data->get_size()+instance_data_size);
        this->m_instance_bounding_spheres->resize_buffer(this->m_instance_bounding_spheres->get_size() + this->m_instance_bounding_spheres->get_layout().get_stride());

        this->m_instance_buffer_data->set_sub_data(data, this->m_instances * instance_data_size, instance_data_size);
        float sphere[] = {position.x, position.y, position.z, radius};
        this->m_instance_bounding_spheres->set_sub_data(sphere, this->m_instances * m_instance_bounding_spheres->get_layout().get_stride(), m_instance_bounding_spheres->get_layout().get_stride());
        this->m_instances ++;
    }

    void Instanced_mesh::on_render() {
        for (auto& lod : this->m_lod) {
           lod.on_render(this->m_frustum_pipeline->fetch_result());
        }
    }

    void Instanced_mesh::on_update() {
        this->m_frustum_pipeline->execute();
    }
}


