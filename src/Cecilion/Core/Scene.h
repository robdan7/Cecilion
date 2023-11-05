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

    private:
        void start() override;


    };

}

