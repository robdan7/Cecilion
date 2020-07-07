#pragma once
#include <fbxdocument.h>
#include <vector>
#include <unordered_map>
#include <Core/Log.h>
#include <Renderer/Buffer.h>
//class FBX_meshes {
//public:
//
//    struct Material {
//        std::vector<int> indices;
//    };
//
//    void createModel(fbx::FBXNode& meshnode) {
//        fbx::FBXNode vertices;
//        fbx::FBXNode normals;
//        fbx::FBXNode vertex_index;
//        fbx::FBXNode material_index;
////        meshnode.print("");
//        if (meshnode.hasChild("Vertices")) {
//            LOG_INFO("Mesh node has vertices");
//            vertices = meshnode.getChildren("Vertices")[0];
//            normals = meshnode.getChildren("LayerElementNormal")[0].getChildren("Normals")[0];
//            LOG_INFO("Vertex size: {0}, normal size: {1}", vertices.getProperties()[0].values.size(), normals.getProperties()[0].values.size());
//            for (int value = 0; value < vertices.getProperties()[0].values.size(); value+=3) {
////            CORE_LOG_INFO("Adding vertex {0}", (float)vertices.getProperties()[0].cast_float(value));
//                for (int i = 0; i < 3; i++) {
//                    this->vertices.push_back((float)vertices.getProperties()[0].cast_float(value+i));        /// Vertices goes here.
//                }
//                for (int i = 0; i < 3; i++) {
////                    LOG_INFO((float)normals.getProperties()[0].cast_float(value+i));
////                    this->vertices.push_back((float)normals.getProperties()[0].cast_float(value+i));        /// Normals goes here.
//                    this->vertices.push_back(1);
//                }
//            }
//        }
//        if (meshnode.hasChild("LayerElementNormal")) {
//            CORE_LOG_INFO("Mesh has normals");
////            normals = meshnode.getChildren("LayerElementNormal")[0].getChildren("Normals")[0];
//            meshnode.getChildren("LayerElementNormal")[0].print("");
////            for (int value = 0; value < normals.getProperties()[0].values.size(); value++) {
//////            CORE_LOG_INFO("Adding vertex {0}", (float)vertices.getProperties()[0].cast_float(value));
////                this->normals.push_back((float)normals.getProperties()[0].cast_float(value));        /// Normals goes here.
////            }
//        }
//        if (meshnode.hasChild("PolygonVertexIndex")) {
//            LOG_INFO("Mesh node has vertex index");
//            vertex_index = meshnode.getChildren("PolygonVertexIndex")[0];
//        }
//        int materials = 0;
//        if (meshnode.hasChild("LayerElementMaterial")) {
//            material_index = meshnode.getChildren("LayerElementMaterial")[0].getChildren("Materials")[0];
//            for (int i = 0; i < material_index.getProperties()[0].values.size();i++) {
//                int mat = material_index.getProperties()[0].cast_int(i);
//                if (mat > materials) {
//                    materials = mat;
//                }
//            }
//            LOG_INFO("Mesh node has {0} materials", materials+1);
//        }
//        materials += 1;
//        this->materials.resize(materials);
//        int vertex_size = 0;
//        for (int value = 0; value < vertex_index.getProperties()[0].values.size(); value++) {
//            vertex_size++;
//            if (vertex_index.getProperties()[0].cast_int(value) < 0) {
//                break;
//            }
//        }
//
//        if (materials && meshnode.getChildren("LayerElementMaterial")[0].getChildren("MappingInformationType")[0].getProperties()[0].to_string() == "ByPolygon") {
//            /// Not tested.
////            if (vertex_index.getProperties()[0].is_array()) {
//
//                for (int x = 0, mindex = 0; x < vertex_index.getProperties()[0].values.size(); x+=vertex_size, mindex++) {
//                    for(int z = 0; z<vertex_size; z++) {
//                        auto val = vertex_index.getProperties()[0].cast_int(x+z);
//                        if (val < 0) {
//                            val = abs(val)-1;
//                        }
//                        /// TODO Put a break point here and check why the material has index -1405460846
//                        CORE_LOG_INFO("Adding vertex index {0} to material {1}", val, material_index.getProperties()[0].cast_int(mindex));
//                        this->materials[material_index.getProperties()[0].cast_int(mindex)].indices.push_back(val);
//                    }
//                }
////            }
//        } else {
//            for (int x = 0, mindex = 0; x < vertex_index.getProperties()[0].values.size(); x++, mindex++) {
//                auto val = vertex_index.getProperties()[0].cast_int(x);
//                if (val < 0) {
//                    val = abs(val)-1;
//                }
//                this->materials[0].indices.push_back(val);
//            }
//        }
//    }
//
//    FBX_meshes(const char* file) {
//        bool flag = false;
//        std::string model = "";
//        std::unordered_map<std::string, fbx::FBXNode*> objects;
//        std::unordered_map<std::string,std::vector<std::string>> links;
//        fbx::FBXDocument doc;
//        doc.read(file);
////        /// Create connections
//        for (fbx::FBXNode node : doc.nodes) {
//            if (node.getName() == "Connections") {
////                LOG_INFO("Found connections");
//                for (fbx::FBXNode& child : node.getChildren("C")) {
//                    if (!links.contains(child.getProperties()[2].to_string())) {
//                        links[child.getProperties()[2].to_string()] = std::vector<std::string>();
//                    }
//                    links[child.getProperties()[2].to_string()].push_back(child.getProperties()[1].to_string());
//                }
//            }
//        }
//        for (fbx::FBXNode& node : doc.nodes) {
//
//            if (node.getName() == "Objects") {
//
//                if(node.hasChild("Model")) {
////                    LOG_INFO("Found model node");
//                    flag = true;
//                    fbx::FBXNode& child = node.getChildren("Model")[0];
//                    model = child.getProperties()[0].to_string();
//                    objects[child.getProperties()[0].to_string()] = &child;
//                }
//                if (node.hasChild("Geometry")) {
//
//                    LOG_INFO("Found geometry node");
//                    fbx::FBXNode& child = node.getChildren("Geometry")[0];
////                    for (auto& child : node.getChildren("Geometry")[0].getChildren("LayerElementMaterial")) {
////                        child.print("");
////                    }
//                    this->createModel(child);
//                    objects[child.getProperties()[0].to_string()] = &child;
//                }
//            }
//        }
//    }
//public:
//    std::vector<float> vertices;
//    std::vector<float> normals;
//    std::vector<Material> materials;
//};

enum class Material_mapping_type {
    ALL_SAME, BY_POLYGON_VERTEX
};
struct FBX_material {
    std::string mapping_type;
    std::vector<uint32_t> vertex_index;
    int material_index;
    int vertices = 0;
    uint32_t starting_vertex = 0;
    FBX_material(std::string mapping_type, int material_index) : mapping_type(mapping_type), material_index(material_index) {}
};

enum class Normal_type {
    BY_POLYGON_VERTEX
};
struct FBX_normals {
    std::string mapping_type = "None";
    std::vector<float> normals;
};

struct FBX_indices {
    int polygon_size;
    std::vector<uint32_t> indices;
};

enum class Draw_type {
    INDEXED, STREAM
};

class FBX_mesh {
public:
    FBX_mesh(fbx::FBXNode& node) {
        fbx::FBXNode* index_node;
        fbx::FBXNode* material_node;

        /// Fill material vector and store the material mesh node.
        if (node.hasChild("LayerElementMaterial")) {
            material_node = &node.getChildren("LayerElementMaterial")[0].getChildren("Materials")[0];
            int material_size = 0;
            std::string mapping_type = node.getChildren("LayerElementMaterial")[0].getChildren("MappingInformationType")[0].getProperties()[0].cast_string();
            for (int i = 0; i < material_node->getProperties()[0].values.size();i++) {
                int mat = material_node->getProperties()[0].cast_int(i);
                if (mat > material_size) material_size = mat;
            }
            /// Create one material for each material index.
            for (int i = 0; i <= material_size; i++) {
                this->materials.emplace_back(mapping_type,i);
            }
        } else {
           this->materials.emplace_back("AllSame",0);   /// Assign a material if none exist.
        }

        /// Fill normal vector.
        if (node.hasChild("LayerElementNormal")) {
            CORE_LOG_INFO("Mesh has normals");
            fbx::FBXNode normals = node.getChildren("LayerElementNormal")[0].getChildren("Normals")[0];
            this->vertex_normals.mapping_type = node.getChildren("LayerElementNormal")[0].getChildren("MappingInformationType")[0].getProperties()[0].cast_string();
            for (int value = 0; value < normals.getProperties()[0].values.size(); value++) {
                this->vertex_normals.normals.push_back((float)normals.getProperties()[0].cast_float(value));
            }
        }

        /// Fill index vector.
        if (node.hasChild("PolygonVertexIndex")) {
            LOG_INFO("Mesh node has vertex index");
            fbx::FBXNode* indices = &node.getChildren("PolygonVertexIndex")[0];
            int polygon_size = 0;
            bool first_polygon = true;
            for (int value = 0; value < indices->getProperties()[0].values.size(); value++) {
                auto val = indices->getProperties()[0].cast_int(value);
                if (first_polygon) polygon_size++;
                if (val < 0) {
                    first_polygon = false;
                    val = abs(val)-1;
                }
                this->indices.indices.push_back(val);
            }
            this->indices.polygon_size = polygon_size;
        }

        /// Fill vertex vector.
        if (node.hasChild("Vertices")) {
            LOG_INFO("Mesh node has vertices");
            fbx::FBXNode* vertex_node = &node.getChildren("Vertices")[0];
            for (int value = 0; value < vertex_node->getProperties()[0].values.size(); value++) {
                this->vertices.push_back((float)vertex_node->getProperties()[0].cast_float(value));
            }
        }
        if (this->vertex_normals.normals.empty()) {
            this->draw_type = Draw_type::INDEXED;
            if (this->materials.size() == 1) {
                /// Only one material. Give all indices to the first one.
                this->materials[0].vertex_index = this->indices.indices;
            } else {
                /// Multiple materials
                for (int polygon = 0; polygon < this->indices.indices.size()/this->indices.polygon_size; polygon++) {
                    int current_material = material_node->getProperties()[0].cast_int(polygon);
                    for (int vertex = 0; vertex < this->indices.polygon_size; vertex++) {
                        this->materials[current_material].vertex_index.push_back(this->indices.indices[polygon*this->indices.polygon_size+vertex]);
                    }
                }
            }
            this->m_vertex_buffer = Cecilion::Vertex_buffer::Create(this->vertices.data(),sizeof(float) * this->vertices.size(),Cecilion::Raw_buffer::Access_frequency::STATIC,Cecilion::Raw_buffer::Access_type::DRAW);
        } else {
            /// TODO The up axis etc should be calculated automatically.
            int upAxis = 2;
            int frontAxis = 0;
            int coordAxis = 1;
            this->draw_type = Draw_type::STREAM;
            std::vector<float> new_vertices;
            int current_material =0;
            std::vector<std::vector<float>> material_vertices;
            material_vertices.resize(this->materials.size());
            for (int polygon = 0; polygon < this->indices.indices.size()/this->indices.polygon_size; polygon++) {
                if (this->materials[0].mapping_type != "AllSame") {
                    current_material = material_node->getProperties()[0].cast_int(polygon);
                }
                ///For each vertex index in a polygon.
                for (int vertex_offset = 0; vertex_offset < this->indices.polygon_size; vertex_offset++) {
                    int vertex_index = polygon*this->indices.polygon_size + vertex_offset;  /// Indices pointer
                    this->materials[current_material].vertices++;
                    this->materials[current_material].vertex_index.push_back(vertex_index);
                    /// For xyz in each vertex.
                    material_vertices[current_material].push_back(this->vertices[this->indices.indices[vertex_index]*this->indices.polygon_size+coordAxis]);
                    material_vertices[current_material].push_back(this->vertices[this->indices.indices[vertex_index]*this->indices.polygon_size+upAxis]);
                    material_vertices[current_material].push_back(this->vertices[this->indices.indices[vertex_index]*this->indices.polygon_size+frontAxis]);

                    material_vertices[current_material].push_back(this->vertex_normals.normals[vertex_index*this->indices.polygon_size+coordAxis]);
                    material_vertices[current_material].push_back(this->vertex_normals.normals[vertex_index*this->indices.polygon_size+upAxis]);
                    material_vertices[current_material].push_back(this->vertex_normals.normals[vertex_index*this->indices.polygon_size+frontAxis]);
                }
            }
            std::vector<float> vertex_buffer;
            int total_vertices = 0;
            for (int i = 0; i < material_vertices.size(); i++) {
                this->materials[i].starting_vertex = total_vertices;
                total_vertices += this->materials[i].vertices;
                vertex_buffer.insert(vertex_buffer.end(),material_vertices[i].begin(),material_vertices[i].end());
            }
            this->m_vertex_buffer = Cecilion::Vertex_buffer::Create(vertex_buffer.data(),sizeof(float)*vertex_buffer.size(),Cecilion::Raw_buffer::Access_frequency::STATIC,Cecilion::Raw_buffer::Access_type::DRAW);
        }
    }
    static FBX_mesh* Create(const char* file) {
        bool flag = false;
        std::string model;
        std::unordered_map<std::string, fbx::FBXNode*> objects;

        fbx::FBXDocument doc;
        doc.read(file);
        std::unordered_map<std::string,std::vector<std::string>> links = fbx::FBXDocument::extract_links(doc);
        FBX_mesh* mesh;
        for (fbx::FBXNode& node : doc.nodes) {

            if (node.getName() == "Objects") {
                if(node.hasChild("Model")) {
                    flag = true;
                    fbx::FBXNode& child = node.getChildren("Model")[0];
                    model = child.getProperties()[0].to_string();
                    objects[child.getProperties()[0].to_string()] = &child;
                }
                if (node.hasChild("Geometry")) {
                    LOG_INFO("Found geometry node");
                    fbx::FBXNode& child = node.getChildren("Geometry")[0];
                    mesh = new FBX_mesh(child);
                    objects[child.getProperties()[0].to_string()] = &child;
                }
            }
        }
        return mesh;
    }

    bool has_normals() const {
        return !this->vertex_normals.normals.empty();
    }
public:
    std::vector<float> vertices;
    FBX_indices indices;
    std::vector<FBX_material> materials;
    Draw_type draw_type;
    FBX_normals vertex_normals;
    std::shared_ptr<Cecilion::Vertex_buffer> m_vertex_buffer;
};