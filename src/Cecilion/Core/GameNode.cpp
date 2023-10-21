#include "GameNode.h"
#include <iostream>
namespace Cecilion {
    GameNode::GameNode(const Cecilion::Entity_ref &entity, const GameNode* parent): I_Component(std::forward<const Entity_ref>(entity)), p_parent(parent) {
        if (this->m_entity == nullptr) {
            std::cout << "Initiated I_Entity" << std::endl;
        } else {
            std::cout << "Entity has already been initialized" << std::endl;
        }
    }

    bool GameNode::operator==(const GameNode &rhs) const {
        return m_entity == rhs.m_entity;
    }

    bool GameNode::operator!=(const GameNode &rhs) const {
        return false;
    }

    bool GameNode::operator==(std::nullptr_t _) {
        return this->m_entity == nullptr;
    }

    bool GameNode::operator!=(std::nullptr_t _) {
        return this->m_entity != nullptr;
    }

    void GameNode::setParent(GameNode *mParent) {
        p_parent = mParent;
    }

    void GameNode::addChild(GameNode &node) {

    }

}

