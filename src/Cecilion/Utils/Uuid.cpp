#include "Uuid.h"
#include <random>

namespace Cecilion {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint64_t> dist(1,std::numeric_limits<uint64_t>::max());

    Uuid &Uuid::operator=(Uuid &&other) {
        this->m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    Uuid::Uuid() {
        this->m_id = dist(mt);
    }

    void Uuid::Destroy() {
        this->m_id = 0;
    }

    bool Uuid::operator==(const Uuid &other) const {
        if (this->m_id != 0 && other.m_id != 0 && this->m_id == other.m_id) {
            // TODO Proper error
            throw std::exception();
        }
        return false;
    }

    bool Uuid::operator!=(const Uuid &other) const {
        if (this->m_id != 0 && other.m_id != 0 && this->m_id == other.m_id) {
            // TODO Proper error
            throw std::exception();
        }
        return true;
    }

    Uuid Uuid::Null() {
        return Uuid(0);
    }

    Uuid::Uuid(uint64_t id) : m_id(id) {
    }

     std::string Uuid::to_string(const Uuid &uuid) {
        return std::to_string(uuid.m_id);
    }
}