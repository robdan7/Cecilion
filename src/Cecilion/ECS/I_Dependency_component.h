#pragma once
#include "I_Component.h"
/*
namespace Cecilion {
    template<class... dependencies>
    class I_Dependency_component : public I_Component {
    protected:
        I_Dependency_component(const Entity_ref& ref): I_Component(ref) {
            ((this->check_dependency<dependencies>())||...);
        }

    private:
        template<class T>
        void check_dependency() {
            if (!this->has_component<T>()) {
                this->add_component<T>();
            }
        }
    };
}*/