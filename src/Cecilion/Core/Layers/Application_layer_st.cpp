#include "Application_layer_st.h"
#include <algorithm>
#include "Layer_stack.h"
#include <Core/Log.h>

Cecilion::Application_layer_st::Application_layer_st() : Application_layer_mt(std::make_shared<Buffered_inbox>(std::shared_ptr<I_Event_actor>(this))) {
//    this->actor_inbox = std::make_shared<Buffered_inbox>(std::shared_ptr<I_Event_actor>(this));
//    this->previous = nullptr;
}

void Cecilion::Application_layer_st::on_update() {
    static_cast<Buffered_inbox*>(this->actor_inbox.get())->execute_inbox();
}