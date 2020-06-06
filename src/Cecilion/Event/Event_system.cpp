#include "Event_system.h"
#include "Event_actor.h"
namespace Cecilion {
    std::unordered_map<std::type_index, I_Event_list*> Event_system::event_containers;
}
