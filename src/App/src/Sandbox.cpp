#include <iostream>
#include <utility>
#include <Cecilion.h>

    struct demo: Cecilion::Event_message {
        string message;
        explicit demo(string message) : Event_message(Cecilion::SYSTEM_STARTUP_EVENT) { this->message = std::move(message);}
        ~demo() override = default;
        //[[nodiscard]] virtual demo* clone() const {return new demo(this->message);}
    };

void test(std::shared_ptr<Cecilion::I_Event_actor> actor, Cecilion::Event_message* message) {
    LOG_INFO("Pressed key!");
}

void test2(std::shared_ptr<Cecilion::I_Event_actor> actor, Cecilion::Event_message* message) {
    LOG_INFO("Pressed mouse key!");
}

class foo:public Cecilion::I_Event_actor {
public:

    void unsub() {
        this->unsubscribe(Cecilion::SYSTEM_STARTUP_EVENT);
    }

    static void call(std::shared_ptr<Cecilion::I_Event_actor> actor, Cecilion::Event_message* mess) {
        LOG_INFO(dynamic_cast<demo*>(mess)->message);
        //dynamic_cast<foo*>(actor.get())->post();
        //std::cout << dynamic_cast<demo*>(mess)->message << std::endl;
    }

    foo() : I_Event_actor("Foo") {
//        this->actor_inbox = new Cecilion::Async_inbox(std::shared_ptr<I_Event_actor>(this));
        this->actor_inbox = std::make_shared<Cecilion::Async_inbox>(std::shared_ptr<I_Event_actor>(this));
        this->subscribe_to(Cecilion::SYSTEM_STARTUP_EVENT, &call);
    }

    void post() {
        std::shared_ptr<Cecilion::Event_message> pointer = std::make_shared<demo>("I have sent a message to myself. Weeeee");
        //demo* m =  new demo("I have sent a message to myself. Weeeee");
        Cecilion::Event_system::post(pointer);
        //Cecilion::Event_message* m = new Cecilion::Event_message(1234);
        //Cecilion::I_Event_actor::post_mt(m);
    }
};

class App : public Cecilion::Application {
public :
    App() {
        foo* f = new foo();
        for (int i = 0; i < 20; i++) {
            if (i == 10) {
                f->unsub();
            }
            f->post();
        }

        std::shared_ptr<Cecilion::Application_layer_st> layer = std::make_shared<Cecilion::Application_layer_st>();
        std::shared_ptr<Cecilion::Application_layer_st> layer2 = std::make_shared<Cecilion::Application_layer_st>();

        this->push_overlay(layer);
        this->push_layer(layer2);
//        layer2->subscribe_to(Cecilion::MOUSE_CURSOR_POS_EVENT, &test2);
        layer->subscribe_to(Cecilion::MOUSE_BUTTON_EVENT, &test2);
        layer2->subscribe_to(Cecilion::KEYBOARD_KEY_EVENT, &test);


        std::shared_ptr<Cecilion::Application_layer_mt> layer3 = std::make_shared<Cecilion::ImGui_layer>();
        this->push_overlay(layer3);
        //LOG_INFO("I have posted a message");

        //f->post();
        //Cecilion::Event_system::CheckMessageStack();

    }

    ~App() {

    }
};


Cecilion::Application* Cecilion::CreateApplication() {
//
//    if (it == layer_list.begin() || it == layer_list.end()) {
//        LOG_CRITICAL("YAAASSDFJDFKJDFLJDFKDSJDSF");
//    }
//    hello.push_back(5);
//    hello.push_back(4);
//    hello.push_back(3);
//    hello.push_back(2);
//    hello.push_back(1);
//    hello.push_back(0);
//
//    for (int i : hello) {
//        LOG_INFO(i);
//    }

    LOG_INFO("Sandbox says hello!");
    return new App();
}