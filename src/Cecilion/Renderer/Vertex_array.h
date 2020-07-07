#pragma once

#include "Buffer.h"
namespace Cecilion {
    /**
     * Just a generic vertex array class.
     */
    class Vertex_array {
    public:
//        enum class Polygon_type {
//            DOT,LINE,TRIANGLE,QUAD
//        };
//        union Render_information {
//            struct Stream {
//                uint32_t m_start_vertex;
//                uint32_t m_vertex_count;
//                uint32_t instances;
//            };
//            struct Indexed {
//                uint32_t elements;
//                uint32_t instances;
//            };
//            Stream stream;
//            Indexed indexed;
//        };
//        enum class Render_ordering {
//            STREAM, INDEXED
//        };

        virtual ~Vertex_array();
        virtual void bind() const =0;
        virtual void unbind() const = 0;
        virtual void add_vertex_buffer(const std::shared_ptr<Vertex_buffer>& vertex_buffer, uint32_t instance_divisor =0) = 0;
        virtual void set_index_buffer(const std::shared_ptr<Index_buffer>& index_buffer) = 0;
        static std::shared_ptr<Vertex_array> Create();
        virtual const std::vector<std::shared_ptr<Vertex_buffer>>& get_vertex_buffer() const = 0;
        virtual const std::shared_ptr<Index_buffer>& get_index_buffer() const = 0;
//        const Render_ordering& get_draw_type() {return this->m_draw_type;}
//        const Polygon_type& get_polygon_type() {return this->m_polygon_type;}
//        const Render_information& get_render_information() {return this->m_render_information;}
//    protected:
//        Render_ordering m_draw_type;
//        Polygon_type m_polygon_type;
//        Render_information m_render_information;
    };
}


