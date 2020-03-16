//#include <iostream>
#include <Cecilion.h>

class App : public Cecilion::Application {
public :
    App() {

    }

    ~App() {

    }
};


Cecilion::Application* Cecilion::CreateApplication() {
    return new App();
}