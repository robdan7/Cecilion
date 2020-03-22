#pragma once
namespace Cecilion {
    struct Event_message {
        unsigned int c_message_ID;
        virtual Event_message* clone() const = 0;
        Event_message(unsigned int ID) : c_message_ID(ID) {}
        virtual ~Event_message() = default;
    };
}

