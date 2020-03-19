#define MACRO __LINE__
//#include <iostream>
//#include "../../Cecilion/Cecilion.h"

#include <iostream>
#include "Cecilion.h"
#define APP_LINE __LINE__


//int main() {
//    Cecilion::Log::Init();
//    CORE_LOG_INFO("Hello world");
//    std::cout << "Hello world" << std::endl;
//}

    struct demo: Cecilion::Event_message{
        string message;
        demo(string message) : Event_message(1234) { this->message = message;}
        ~demo() {};
    };


class foo:public Cecilion::I_Event_actor {
public:

    static void call(Cecilion::I_Event_actor* actor, Cecilion::Event_message* mess) {
        LOG_INFO(dynamic_cast<demo*>(mess)->message);
        //std::cout << "hejj" << std::endl;
    }

    foo() : I_Event_actor("Foo") {
        this->subscribe_to(1234, &call);
    }

    void post() {
        demo* m = new demo("I have sent a message to myself. Weeeee");
        Cecilion::I_Event_actor::post(m);
        //Cecilion::Event_message* m = new Cecilion::Event_message(1234);
        //Cecilion::I_Event_actor::post_mt(m);
    }
};

class App : public Cecilion::Application {
public :
    App() {
        foo* f = new foo();
        f->post();

        //Cecilion::Event_system::CheckMessageStack();

    }

    ~App() {

    }
};


Cecilion::Application* Cecilion::CreateApplication() {
    LOG_INFO("Sandbox says hello!");
    return new App();
}