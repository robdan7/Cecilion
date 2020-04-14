#pragma once
#include "Application_layer_mt.h"
#include <Event/Buffered_inbox.h>
namespace Cecilion {

    /**
     * The single threaded application layer is a dumbed down version of the multi threaded
     * application layer. the only difference is the on_update method.
     */
    class Layer_stack;
    class Application_layer_st : public Application_layer_mt {
    public:
        Application_layer_st();
        void on_update() override;
    };
}

