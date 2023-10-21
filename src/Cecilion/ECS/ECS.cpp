#include "ECS.h"
#include "Config.h"

#include <Core/Log.h>
#include <iostream>
namespace Cecilion {
    ECS::~ECS() {
        for (auto it = m_component_storage.begin(); it != m_component_storage.end(); ++it) {
            delete it->second;
        }
    }

    void ECS::delete_entity(const ECS::Entity_ID& ID) {
        if (!this->m_entities.has_entry(ID)) {
            // TODO Error
            return;
        }

        if (this->m_entities[ID].m_refs > 0) {
            // TODO Error
        }

        this->m_entities.free(ID);
    }



//	void ECS::set_as_active_context() {
//		this = (ECS*)this;
//	}

    void ECS::clear_storage() {
        /*
        for (auto& ID : this->m_delete_list) {
            if (std::find(this->m_freelist.begin(), this->m_freelist.end(), ID) != this->m_freelist.end()) {
                // ORVOX_ERROR("Tried to delete an entity that does not exist"); TODO Error
            }
            for (auto it = this->m_component_storage.begin(); it != this->m_component_storage.end(); ++it) {
                it->second->try_delete(ID);
            }
            this->m_freelist.push_back(ID);
        }
        this->m_delete_list.clear();*/
    }


    Entity_ref ECS::create_entity() {

        // This may throw error.
        auto id = this->m_entities.alloc();

        this->m_entities.at(id).m_entity_ID = id;

        return Entity_ref(this, &this->m_entities[id]);
    }
}