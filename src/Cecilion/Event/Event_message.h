#pragma once
namespace Cecilion {
    struct Event_message {
        unsigned const int c_message_ID;

        Event_message(unsigned int ID) : c_message_ID(ID) {}
        virtual ~Event_message() = default;
    };
}

