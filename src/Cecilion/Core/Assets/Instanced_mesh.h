#pragma once
#include <Renderer/Buffer.h>
#include <memory>
#include <initializer_list>
#include <Renderer/Frustum_compute_query.h>
#include <Renderer/Shader.h>
#include <Renderer/Vertex_array.h>
namespace Cecilion {
    class LOD {

    };

    class Instanced_mesh {
        struct Mesh_data {
            std::shared_ptr<Vertex_buffer> p_buffer;
            std::shared_ptr<Index_buffer> p_index_buffer;
        };

        class Mesh_LOD {
        public:
            Mesh_LOD(Mesh_data mesh, std::shared_ptr<Shader> shader, std::shared_ptr<Frustum_compute_query> frustum_pipeline);
            void on_render();
        private:
            std::shared_ptr<Shader> m_shader;
            std::shared_ptr<Vertex_array> m_vao;
            std::shared_ptr<Frustum_compute_query> m_frustum_pipeline;
        };

    public:
        Instanced_mesh(const std::initializer_list<Buffer_element>& instance_layout);

        void add_LOD(std::shared_ptr<Vertex_buffer> vertex_data, std::shared_ptr<Index_buffer> index_buffer, std::shared_ptr<Shader> shader);

        void on_update();

        void on_render();

        void add_instance(float* data, glm::vec3 position, float radius);
    public:
        std::shared_ptr<Frustum_compute_query> m_frustum_pipeline;
    private:
        std::vector<Mesh_LOD> m_lod;
        std::shared_ptr<Vertex_buffer> m_instance_buffer_data;  // uncompressed instance storage.
        std::shared_ptr<Vertex_buffer> m_instance_bounding_spheres;
//        std::vector<float> m_instance_data;
        uint32_t instances;

    };

}

