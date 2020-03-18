#define MACRO __LINE__
//#include <iostream>
#include "../../Cecilion/Cecilion.h"
#include <iostream>

#define APP_LINE __LINE__


    struct demo: Cecilion::EventMessage{
        string message;
        demo(string message) : EventMessage(1234) { this->message = message;}
        ~demo() {};
    };


class foo:public Cecilion::I_EventActor {
public:

    static void call(Cecilion::I_EventActor* actor, Cecilion::EventMessage* mess) {
        LOG_INFO(dynamic_cast<demo*>(mess)->message);
        //std::cout << "hejj" << std::endl;
    }

    foo() : I_EventActor("Foo") {
        this->SubscribeTo(1234, &call);
    }

    void post() {
        demo* m = new demo("I have sent a message to myself. Weeee");
        Cecilion::I_EventActor::post(m);
        //Cecilion::EventMessage* m = new Cecilion::EventMessage(1234);
        //Cecilion::I_EventActor::post_mt(m);
    }
};

class App : public Cecilion::Application {
public :
    App() {

        foo* f = new foo();
        f->unsubscribe(1234);
        f->post();

        //Cecilion::EventSystem::CheckMessageStack();

    }

    ~App() {

    }
};


Cecilion::Application* Cecilion::CreateApplication() {
    LOG_INFO("Sandbox says hello! ");
    return new App();
}