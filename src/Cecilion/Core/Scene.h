#pragma once
#include <ECS/ECS.h>
#include "GameNode.h"
#include <iostream>


namespace Cecilion {
    class I_Scene {
    protected:
        I_Scene() = default;
        Cecilion::ECS m_ecs;
    };

    class Scene: I_Scene, Cecilion::GameNode {
    public:
        explicit Scene() : GameNode(this->m_ecs.create_entity(), nullptr) {
            std::cout << "Scene start" << std::endl;
        }
        

    private:
        void start() override {
            std::cout << "Scene started!!" << std::endl;
        }


    };

}

