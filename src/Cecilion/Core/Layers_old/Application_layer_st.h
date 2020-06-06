#pragma once
#include "Application_layer_mt.h"
//#include <Event/Buffered_inbox.h>
namespace Cecilion {

    /**
     * The single threaded application layer is a dumbed down version of the multi threaded
     * application layer. the only difference is the on_update method.
     */
     template<typename... Events>
    class Layer_stack;

    template<typename... Events>
    class Application_layer_st : public Application_layer_mt<Events...> {
    public:
        Application_layer_st();
        void on_update() {

        };
    };
}

