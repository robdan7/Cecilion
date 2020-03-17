#define MACRO __LINE__
//#include <iostream>
#include "../../Cecilion/Cecilion.h"
#include <iostream>

#define APP_LINE __LINE__

class App : public Cecilion::Application {
public :
    App() {

    }

    ~App() {

    }
};


Cecilion::Application* Cecilion::CreateApplication() {
    LOG_INFO("Sandbox says hello! ");
    return new App();
}