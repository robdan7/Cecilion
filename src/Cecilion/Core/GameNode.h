#pragma once
#include <ECS/ECS.h>
#include <ECS/Config.h>
#include <ECS/Entity.h>
#include <ECS/Component.h>
#include <yaml-cpp/yaml.h>
#include <vector>
#include "Transform.h"
namespace Cecilion {
    class GameNode: public I_Dependency_component<Cecilion::Transform> {
    public:
        GameNode(const GameNode&) = delete;
        GameNode& operator=(const GameNode&) = delete;
        GameNode() = delete;

        [[deprecated]]
        I_Component_ref parse_component(const YAML::Node& node) {
            return this->m_entity.add_component(node);
        }
        void setParent(Component_ref<GameNode>& new_parent);

        Cecilion::Component_ref<Transform> transform();

    protected:
        explicit GameNode(const Cecilion::Entity_ref& entity, const Component_ref<GameNode>& parent);
        explicit GameNode(const Cecilion::Entity_ref& entity);

    private:
        virtual void start(){}
        virtual void active(){}
        virtual void inactive(){}
        virtual void update(){}
        virtual void destroy(){}

    private:
        Component_ref<GameNode> m_parent;
        std::vector<Component_ref<GameNode>> m_children;
        Component_ref<Transform> m_transform;
    };
}