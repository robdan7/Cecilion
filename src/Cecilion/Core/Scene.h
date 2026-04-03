#pragma once
#include "GameNode.h"
#include <iostream>


namespace Cecilion {
    class ECS;
    class I_Scene {
    public:
        I_Scene();
        std::shared_ptr<Cecilion::ECS> p_ecs;
    };

    class Scene: protected I_Scene, public Cecilion::GameNode {
    public:
        explicit Scene();
        //YAML::Node serialize();
        //void parse_component(const YAML::Node& node);
        template<typename T>
        bool has_component() {
            return this->entity().has_component<T>();
        }

        template<typename C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        Component_ref<C> get_component() {
            return this->entity().get_component<C>();
        }


    private:
        void start() override;
    };

}

