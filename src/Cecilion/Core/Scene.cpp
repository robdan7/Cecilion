#include <ECS/ECS.h>
#include "Scene.h"
namespace Cecilion {
    Scene::Scene(): GameNode(this->p_ecs->create_entity()) {

    }

    void Scene::start() {
        std::cout << "Scene started" << std::endl;

    }

    I_Scene::I_Scene() : p_ecs(new Cecilion::ECS()){

    }
} // Cecilion