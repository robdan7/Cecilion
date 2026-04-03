#pragma once
#include "I_Component.h"
#include "Config.h"
#include <Utils/Uuid.h>
/*
namespace Cecilion {
    class Entity_metadata : public I_Component {
    public:
        friend Entity_ref;
        Entity_metadata(const Entity_ref& ref): I_Component(ref) {}

        bool operator==(std::nullptr_t _) const {
            return this->m_entity_ID == ECS_NULL_ENTITY;
        }

        bool operator!=(std::nullptr_t _) const {
            return this->m_entity_ID != ECS_NULL_ENTITY;
        }

        bool operator==(const Entity_metadata& other) const;
        bool operator!=(const Entity_metadata& other) const;
    private:
        ECS_ENTITY_SIZE m_entity_ID = ECS_NULL_ENTITY;
        std::size_t m_refs = 0;
        Cecilion::Uuid m_uuid = Uuid(); // Holds UUID for serialization and entity traceability.
    };
}*/