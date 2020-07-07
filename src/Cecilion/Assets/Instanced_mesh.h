#pragma once
#include <Renderer/Buffer.h>
#include <memory>
#include <initializer_list>
#include <Renderer/Frustum_compute_query.h>
#include <Renderer/Shader.h>
#include <Renderer/Vertex_array.h>
#include <Renderer/Render_encoder.h>
namespace Cecilion {
    /**
     * Demo class for mesh instancing. This is a work in progress.
     */
    class Instanced_mesh {
    private:
        struct Material {
//            std::shared_ptr<Index_buffer> p_index_buffer;
            std::shared_ptr<Vertex_array> m_vao;
            std::shared_ptr<Shader> m_shader;
            Render::Render_encoder* m_renderer;
            Material(std::shared_ptr<Vertex_array> array, std::shared_ptr<Shader> shader) : m_vao(array) , m_shader(shader){
                this->m_renderer = new Render::Index_renderer(Render::Polygon_type::TRIANGLE);
            }
            Material(std::shared_ptr<Vertex_array> array, std::shared_ptr<Shader> shader, uint32_t start_vertex, uint32_t vertex_count) : m_vao(array) , m_shader(shader){
                this->m_renderer = new Render::Stream_renderer(Render::Polygon_type::TRIANGLE,start_vertex,vertex_count);
            }
            void draw(uint32_t instances) {
                this->m_renderer->draw_instanced(this->m_shader,this->m_vao,instances);
            }
            ~Material() {
                delete this->m_renderer;
            }
        };
        struct Mesh_data {
            std::shared_ptr<Vertex_buffer> p_buffer;
            std::vector<Material> m_materials;
            Mesh_data(std::shared_ptr<Vertex_buffer> vertices) : p_buffer(vertices) {}
            void on_render(uint32_t instances) {
                for (auto& material : this->m_materials) {
                    material.draw(instances);
                }
            }
        };
    public:
        Instanced_mesh(const std::initializer_list<Buffer_element>& instance_layout);

        /**
         * Add another level of detail. Not finished yet.
         * @param vertex_data
         * @param index_buffer
         * @param shader
         */
//        void add_LOD(std::shared_ptr<Vertex_buffer> vertex_data, std::shared_ptr<Index_buffer> index_buffer);

        void add_LOD(const char* file, const std::shared_ptr<Shader>& shader);

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
        std::vector<Mesh_data> m_lod;    /// List of all LODs.
        std::shared_ptr<Vertex_buffer> m_instance_buffer_data;  /// uncompressed instance storage.
        std::shared_ptr<Vertex_buffer> m_instance_bounding_spheres; /// Bounding spheres used for future frustum culling.
        uint32_t m_instances{};
    };

}

