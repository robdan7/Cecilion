#pragma once
#include <ECS/Config.h>
#include <vector>
namespace Cecilion {
    class Scene {
    private:
        class Scene_node {
            Entity m_entity;
            const Scene_node& m_parent;
            Scene_node(const Scene_node& parent) : m_parent(parent) {}
            std::vector<Scene_node> m_children;
        };
        Scene(const char* file);
    public:


    };
}


