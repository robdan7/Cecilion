#pragma once

#include <type_traits>
#include "Entity.h"
namespace Cecilion {
    class I_Component {
        friend class ECS;
    public:
        I_Component(I_Component&& other) noexcept : m_entity(std::move(other.m_entity)) {}

        I_Component& operator=(I_Component&& other)  noexcept {
            this->m_entity.operator=(std::move(other.m_entity));
        }
    protected:
        Entity_ref m_entity;
        explicit I_Component(const Entity_ref& ref): m_entity(std::forward<const Entity_ref>(ref)){}
    };

    class I_Component_ref {
    protected:
        explicit I_Component_ref(const Entity_ref& entity): m_entity(std::forward<const Entity_ref>(entity)){}

    private:
        template<class C, std::enable_if<std::is_base_of<I_Component, C>::value>::type * = nullptr>
        void destroy() {

        }
        const Entity_ref m_entity;
    };

    template<class C, std::enable_if<std::is_base_of<I_Component, C>::value>::type* = nullptr>
    class Component_ref: I_Component_ref {
        friend class ECS;

        C &operator->() {
            if (this->operator==(nullptr)) {
                // TODO Error
            }
            return this->m_entity.template get_component<C>();
        }

        bool operator==(std::nullptr_t _) {
            return this->m_entity == nullptr;
        }
        bool operator!=(std::nullptr_t _) {
            return this->m_entity != nullptr;
        }

        Entity_ref entity() {
            return this->m_entity;
        }

        /**
         * Add a component of type T to the owner of this component.
         * @tparam T
         * @tparam Args
         * @param args
         * @return
         */
        template<class T,  std::enable_if<std::is_base_of<I_Component, C>::value>::type* = nullptr, typename... Args>
        Component_ref<T> add_component(Args... args) {

        }

        /**
         * Get a component of type T
         * @tparam T
         * @return
         */
        template<class T,  std::enable_if<std::is_base_of<I_Component, T>::value>::type* = nullptr>
        Component_ref<T> get_component() {
            return static_cast<T&>(this->m_entity.get_component(typeid(T)));
        }

    private:
        explicit Component_ref(const Entity_ref &entity): I_Component_ref(std::forward<const Entity_ref>(entity)){}
    };
}