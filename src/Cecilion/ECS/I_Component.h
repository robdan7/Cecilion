#pragma once
#include "Entity_ref.h"
/*
namespace Cecilion {

    class I_Component {
        friend class ECS;
        friend class I_Component_ref;
    public:
        I_Component(I_Component&& other) noexcept;
        I_Component& operator=(I_Component&& other)  noexcept;
        virtual ~I_Component() = default;

        bool operator ==(const std::nullptr_t & _) const;

        Entity_ref& entity();

        void Destroy();

        template<class C>
        Component_ref<C> get_component();

        template<class C>
        Component_ref<C> add_component();

        template<class C>
        bool has_component() const;
        //I_Component_ref parse_component(const YAML::Node& node);

    protected:
        explicit I_Component(const Entity_ref& ref);
    private:
        Entity_ref m_entity;
        std::size_t m_refs = 0;
    };

    class I_Component_ref {
        friend class ECS;
    public:
        Entity_ref& entity() {
            return this->m_entity;
        }
    protected:
        I_Component_ref(){}
        template<class C>
        C& get_component_source() {
            return this->m_entity.get_component_source<C>();
        }
        template<class C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        std::size_t add_source_ref() {
            return ++ static_cast<I_Component&>(this->get_component_source<C>()).m_refs;
        }

        template<class C, std::enable_if_t<std::is_base_of_v<I_Component, C>> * = nullptr>
        std::size_t subtract_source_ref() {
            return -- static_cast<I_Component&>(this->get_component_source<C>()).m_refs;
        }
        explicit I_Component_ref(const Entity_ref& entity);
        Entity_ref m_entity;

    private:
        template<class C>
        static I_Component_ref deserialize_helper(ECS * ecs, const YAML::Node& node, const Entity_ref &entity);
    };

    template<class C>
    class Component_ref: I_Component_ref {
    public:
        friend class ECS;
        friend class I_Component_ref;
        friend class Entity_ref;

        Component_ref(){}

        ~Component_ref() {
            if (this->m_entity != nullptr) {
                this->subtract_source_ref<C>();
                this->m_entity = nullptr;
            }
        }

        Component_ref& operator=(const std::nullptr_t& _) {
            if (this->m_entity != nullptr) {
                this->subtract_source_ref<C>();
                this->m_entity = nullptr;
            }
            return *this;
        }

        C &operator->() {
            if (this->operator==(nullptr)) {
                // TODO Error
                throw std::exception();
            }
            return this->get_component_source<C>();
        }
        bool operator==(std::nullptr_t _) {
            return this->m_entity == nullptr || this->has_component<C>() == false;
        }

        bool operator!=(std::nullptr_t _) {
            return this->m_entity != nullptr;
        }

        bool operator==(const Component_ref<C>& other) {
            return this->m_entity == other.m_entity;
        }

        template<class T>
        Component_ref<T> add_component() {
            return this->m_entity.add_component<T>();
        }

        template<class T>
        Component_ref<T> get_component() {
            return this->m_entity.get_component<T>();
        }

        template<typename T>
        bool has_component() const {
            return this->m_entity.has_component<T>();
        }
/*
        YAML::Node serialize() override {
            YAML::Node n;
            n[Cecilion::Serializable::s_type_declaration] = Cecilion::type(*this);
            if (this->operator!=(nullptr)) {
                n["m_entity"] = this->m_entity.operator->().m_entity_ID;
            }
            return n;
        }

        Component_ref<C> &operator=(const YAML::Node &serializedNode) override {
            if (serializedNode["m_entity"]) {
                this->m_entity.operator->().m_entity_ID = serializedNode["m_entity"].as<ECS_ENTITY_SIZE>();
            }
            return *this;
        }
*/
/*
    private:
        explicit Component_ref(const Entity_ref &entity): I_Component_ref(std::forward<const Entity_ref>(entity)){
            this->add_source_ref<C>();
        }

    };



    template<class C>
    Component_ref<C> I_Component::get_component() {
        return this->m_entity.get_component<C>();
    }

    template<class C>
    bool I_Component::has_component() const {
        return this->m_entity.has_component<C>();
    }

    template<class C>
    I_Component_ref I_Component_ref::deserialize_helper(ECS * ecs, const YAML::Node& node, const Entity_ref &entity) {
        return Component_ref<C>(entity);
    }
}
*/