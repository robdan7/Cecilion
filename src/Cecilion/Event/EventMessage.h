#ifndef CECILION_EVENTMESSAGE_H
#define CECILION_EVENTMESSAGE_H
namespace Cecilion {
    struct EventMessage {
        unsigned int messageID;

        EventMessage(unsigned int ID) {this->messageID = ID;}
        virtual ~EventMessage() = default;
    };
}
#endif //CECILION_EVENTMESSAGE_H
