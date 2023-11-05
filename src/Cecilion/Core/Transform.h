#pragma once
#include <ECS/ECS.h>
#include <Utils/Serializable.h>
#include <Utils/Type.h>

namespace Cecilion {
    class Transform: public Cecilion::I_Component, public Cecilion::Serializable {
        friend class ECS;
    public:
        Transform(const Cecilion::Entity_ref& ref): Cecilion::I_Component(ref) {
            std::cout << "Creating transform for component" << std::endl;
        }
        Transform(Transform&& other): Cecilion::I_Component(std::move(other)), x(other.x), y(other.y), z(other.z) {

        }

        Transform& operator=(Transform&& other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            return *this;
        }

        ~Transform() {}

    private:
        YAML::Node serialize() override {
            YAML::Node n;
            n[Cecilion::Serializable::s_type_declaration] = Cecilion::type(*this);
            n["x"] = this->x;
            n["y"] = this->y;
            n["z"] = this->z;
            return n;
        }

        Serializable &operator=(const YAML::Node &serializedNode) override {
            this->x = serializedNode["x"].as<float>();
            this->y = serializedNode["y"].as<float>();
            this->z = serializedNode["z"].as<float>();
            return *this;
        }

        [[deprecated]]
        float x,y,z;
    };
}

