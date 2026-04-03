#include "I_Component.h"
/*
namespace Cecilion {
    bool I_Component::operator==(const std::nullptr_t& _) const {
        return this->m_entity.operator==(nullptr);
    }

    void I_Component::Destroy() {
        this->m_entity = nullptr;
    }

    I_Component::I_Component(I_Component &&other) noexcept : m_entity(std::move(other.m_entity)) {
        other.m_entity = nullptr;
    }

    I_Component &I_Component::operator=(I_Component &&other) noexcept {
        this->m_entity.operator=(std::move(other.m_entity));
        return *this;
    }

    Entity_ref &I_Component::entity()  {
        return this->m_entity;
    }

    I_Component::I_Component(const Entity_ref &ref): m_entity(std::forward<const Entity_ref>(ref)){

    }

}*/