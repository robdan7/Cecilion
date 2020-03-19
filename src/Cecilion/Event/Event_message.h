#ifndef CECILION_EVENT_MESSAGE_H
#define CECILION_EVENT_MESSAGE_H
namespace Cecilion {
    struct Event_message {
        unsigned const int c_message_ID;

        Event_message(unsigned int ID) : c_message_ID(ID) {}
        virtual ~Event_message() = default;
    };
}
#endif //CECILION_EVENT_MESSAGE_H
