#pragma once
#include <Renderer/Buffer.h>
#include <memory>
#include <initializer_list>
#include <Renderer/Frustum_compute_query.h>
#include <Renderer/Shader.h>
#include <Renderer/Vertex_array.h>
namespace Cecilion {

    /**
     * Demo class for mesh instancing. This is a work in progress.
     */
    class Instanced_mesh {
        struct Mesh_data {
            std::shared_ptr<Vertex_buffer> p_buffer;
            std::shared_ptr<Index_buffer> p_index_buffer;
        };

        /**
         * Each LOD contains a mesh and a buffer (currently a frustum compute query) for all it's instances.
         * Work in progress.
         */
        class Mesh_LOD {
        public:
            Mesh_LOD(const Mesh_data& mesh, std::shared_ptr<Shader> shader, std::shared_ptr<Frustum_compute_query> frustum_pipeline);
            void on_render();
        private:
            std::shared_ptr<Shader> m_shader;
            std::shared_ptr<Vertex_array> m_vao;
            std::shared_ptr<Frustum_compute_query> m_frustum_pipeline;
        };

    public:
        Instanced_mesh(const std::initializer_list<Buffer_element>& instance_layout);

        /**
         * Add another level of detail. Not finished yet.
         * @param vertex_data
         * @param index_buffer
         * @param shader
         */
        void add_LOD(std::shared_ptr<Vertex_buffer> vertex_data, std::shared_ptr<Index_buffer> index_buffer, const std::shared_ptr<Shader>& shader);

        /**
         * This should be part of an entity component. The purpose is to update all LODs.
         */
        void on_update();

        void on_render();

        /**
         * Add another instance of this mesh. This is also a work in progress.
         * @param data
         * @param position
         * @param radius
         */
        void add_instance(float* data, glm::vec3 position, float radius);
    public:
        std::shared_ptr<Frustum_compute_query> m_frustum_pipeline;
    private:
        std::vector<Mesh_LOD> m_lod;    /// List of all LODs.
        std::shared_ptr<Vertex_buffer> m_instance_buffer_data;  /// uncompressed instance storage.
        std::shared_ptr<Vertex_buffer> m_instance_bounding_spheres; /// Bounding spheres used for future frustum culling.
        uint32_t m_instances{};

    };

}

