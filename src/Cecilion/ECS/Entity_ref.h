#pragma once
#include "Config.h"
//#include "ECS.h"
/*
namespace Cecilion {
    class I_Component;

    class I_Component_ref;

    template<typename C>
    class Component_ref;

    class Entity_ref {
        friend class ECS;
        friend class I_Component_ref;
    public:
        using Entity_ID = ECS_ENTITY_SIZE;

        //YAML::Node serialize() override;

        //Serializable &operator=(const YAML::Node &serializedNode) override;

        Entity_ref(Entity_ref&& other) noexcept;

        Entity_ref(const Entity_ref& other) noexcept;

        Entity_ref(){}

        Entity_ref& operator=(const Entity_ref& other);

        Entity_ref& operator=(Entity_ref&& obj)  noexcept;

        Entity_ref& operator=(std::nullptr_t&& _);

        ~Entity_ref();

        void Destroy() const;

        bool operator==(const Entity_ref &rhs) const;

        bool operator==(const std::nullptr_t& _) const;

        bool operator!=(const Entity_ref &rhs) const {
            return this->m_source != rhs.m_source;
        }

        bool operator!=(const std::nullptr_t& _) const;

        //Entity_source& operator->();


        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        Component_ref<C> get_component() {
            return Component_ref<C>(*this);
        }

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        Component_ref<C> add_component();

        I_Component_ref add_component(const YAML::Node& node) const;

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        bool has_component() const {
            return this->p_ecs->has_component<C>(this->id());
        }

        template<typename C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        void remove_component() const {
            this->p_ecs->pop_component<C>(this->id());
        }

    private:
        template<class C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        C &get_component_source() {
            return this->p_ecs->get_component<C>(this->id());
        }

        [[nodiscard]] const Entity_ID& id() const;

        explicit Entity_ref(ECS* ecs, const ECS_ENTITY_SIZE &source);

        ECS* p_ecs = nullptr;
        ECS_ENTITY_SIZE m_source = ECS_NULL_ENTITY;
    };


}
*/