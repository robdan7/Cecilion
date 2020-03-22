#include <iostream>
#include <utility>
#include "Cecilion.h"
#include "Event_inbox.h"

    struct demo: Cecilion::Event_message{
        string message;
        explicit demo(string message) : Event_message(1234) { this->message = std::move(message);}
        ~demo() override = default;
        [[nodiscard]] virtual demo* clone() const {return new demo(this->message);}
    };


class foo:public Cecilion::I_Event_actor {
public:

    static void call(std::shared_ptr<Cecilion::I_Event_actor> actor, Cecilion::Event_message* mess) {
        LOG_INFO(dynamic_cast<demo*>(mess)->message);
        //dynamic_cast<foo*>(actor.get())->post();
        //std::cout << dynamic_cast<demo*>(mess)->message << std::endl;
    }

    foo() : I_Event_actor("Foo") {
        this->subscribe_to(1234, &call);
    }

    void post() {
        std::shared_ptr<Cecilion::Event_message> pointer = std::make_shared<demo>("I have sent a message to myself. Weeeee");
        //demo* m =  new demo("I have sent a message to myself. Weeeee");
        Cecilion::I_Event_actor::post(pointer);
        //Cecilion::Event_message* m = new Cecilion::Event_message(1234);
        //Cecilion::I_Event_actor::post_mt(m);
    }
};

class App : public Cecilion::Application {
public :
    App() {
        foo* f = new foo();
        for (int i = 0; i < 20; i++) {
            f->post();
        }
        LOG_INFO("I have posted a message");
        f->unsubscribe(1234);
        //f->post();
        //Cecilion::Event_system::CheckMessageStack();

    }

    ~App() {

    }
};


Cecilion::Application* Cecilion::CreateApplication() {
    LOG_INFO("Sandbox says hello!");
    return new App();
}