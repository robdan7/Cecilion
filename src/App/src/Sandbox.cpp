#include "Sandbox.h"
#include <string>
#include <Utils/Type.h>
#include <pfr.hpp>

struct Test_component : Cecilion::I_Component, Cecilion::Serializable {
public:
    explicit Test_component(const Cecilion::Entity_ref& ref): Cecilion::I_Component(std::forward<const Cecilion::Entity_ref>(ref)) {}
    Test_component& operator=(Test_component&& other ){
        return *this;
    }
    Test_component(Test_component&& other) : Cecilion::I_Component(std::move(other)), my_number(other.my_number){}

    ~Test_component() {
        std::cout << "Destroyed test component! My number is " << this->my_number << std::endl;
    }

    YAML::Node serialize() override {
        YAML::Node n;
        n[Cecilion::Serializable::s_type_declaration] = Cecilion::type(*this);
        n["my_number"] = this->my_number;
        return n;
    }

    Test_component &operator=(const YAML::Node &serializedNode) override {
        this->my_number = serializedNode["my_number"].as<int>();
        return *this;
    }

    int my_number{};
};

// This registers the test component as a serializable object at runtime.
REGISTER_SERIALIZABLE_COMPONENT(Test_component);


class App : public Cecilion::Application {
public :
    App() {

        {
            Cecilion::Scene scene;
            try {
                // This will parse a component from yaml string. Note the value of my_number
                scene.parse_component(YAML::Load("{typeDecl: Test_component, my_number: 1}"));
            } catch (std::exception e) {
                std::cout << "Could not create component from yaml" << std::endl;
            }

            // All gameNodes have a transform
            std::cout << "Scene has transform component? " << ((scene.transform() != nullptr) ? "Yes" :"No") << std::endl;

            std::cout << scene.entity().serialize() << std::endl;

            if (scene.has_component<Test_component>()) {
                auto ref = scene.get_component<Test_component>();
                ref.operator->().my_number = 123;

                scene.entity().Destroy();
                std::cout << "Is scene null? " << ((scene.entity() == nullptr) ? "Yes": "No") << std::endl;
                std::cout << "Is scene component null? " << ((scene == nullptr) ? "Yes" : "No") << std::endl;
                std::cout << "Is the component null? " << ((ref == nullptr) ? "Yes": "No") << std::endl;

            } else {
                std::cout << "Could not find Test_component in the scene node" << std::endl;
            }
        }
        std::cout << "Application has finished executing. You may shut it down now :)" << std::endl;
    }

    ~App() override {

    }
};



Cecilion::Application *Cecilion::CreateApplication() {
    return new App();
}
