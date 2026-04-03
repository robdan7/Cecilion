#pragma once
#include <ECS/ECS.h>
#include <ECS/I_Dependency_component.h>
#include <yaml-cpp/yaml.h>
#include "Transform.h"
namespace Cecilion {
    class GameNode: public I_Dependency_component<Cecilion::Transform> {
        friend ECS;
    public:
        GameNode(GameNode&& other);
        explicit GameNode(const Cecilion::Entity_ref& entity);

        GameNode& operator=(GameNode&& other);
        GameNode(const GameNode&) = delete;
        GameNode& operator=(const GameNode&) = delete;
        GameNode() = delete;

        ~GameNode() {}
        [[deprecated]]
        I_Component_ref parse_component(const YAML::Node& node) {
            return this->entity().add_component(node);
        }

        Cecilion::Component_ref<Transform> transform();

        YAML::Node serialize();

        Serializable& operator=(const YAML::Node& node);

    private:
        virtual void start(){}
        virtual void active(){}
        virtual void inactive(){}
        virtual void update(){}
        virtual void destroy(){}

        Component_ref<Transform> m_transform;
    };
}