#pragma once
#include <cstdint>
#include <string>
namespace Cecilion {

    class Uuid {
    public:
        Uuid();
        Uuid& operator=(Uuid&& other);
        Uuid(const Uuid& other) = delete;
        Uuid& operator=(const Uuid& other) = delete;
        void Destroy();
        bool operator==(const Uuid& other) const;
        bool operator!=(const Uuid& other) const;
        static Uuid Null();
        static std::string to_string(const Uuid& uuid);
    private:
        explicit Uuid(uint64_t id);
        uint64_t m_id = 0;
    };

}
