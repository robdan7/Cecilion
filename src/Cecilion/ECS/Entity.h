#pragma once
#include <cstddef>
#include "Config.h"
namespace Cecilion {
    class I_Component;
    class I_Component_ref;


    struct Entity_ref {
        friend class ECS;
        friend class I_Component_ref;
    public:
        using Entity_ID = ECS_ENTITY_SIZE;

        Entity_ref(Entity_ref&& other) noexcept;

        Entity_ref(const Entity_ref& other) noexcept;

        Entity_ref& operator=(const Entity_ref& other);

        Entity_ref& operator=(Entity_ref&& obj)  noexcept;

        ~Entity_ref();

        bool operator==(const Entity_ref &rhs) const {
            return this->p_source == rhs.p_source;
        }

        bool operator==(const std::nullptr_t& _) const {
            // Check if either pointer or source is null
            return this->p_source == nullptr || this->p_source->operator==(nullptr);
        }

        bool operator!=(const Entity_ref &rhs) const {
            return this->p_source != rhs.p_source;
        }

        bool operator!=(const std::nullptr_t& _) const {
            // Check both pointer and value
            return  this->p_source != nullptr && this->p_source->operator!=(nullptr);
        }

        Entity_ref Instantiate();

        template<typename C>
        C& get_component();

        [[nodiscard]] auto id() const {
            return this->p_source->m_entity_ID;
        }

        ECS* get_ecs();

    private:
        struct Entity_source {
            bool operator == (std::nullptr_t _) const {
                return this->m_entity_ID == ECS_NULL_ENTITY;
            }
            bool operator !=(std::nullptr_t _) const {
                return this->m_entity_ID != ECS_NULL_ENTITY;
            }
            Entity_ID m_entity_ID = ECS_NULL_ENTITY;
            std::size_t m_refs = 0;
        };

        void delete_compoent(std::type_index type);

        I_Component& get_component(std::type_index type);

        explicit Entity_ref(ECS* ecs, Entity_source* source);

        Entity_source* p_source = nullptr;
        ECS* p_ecs;

    };
}