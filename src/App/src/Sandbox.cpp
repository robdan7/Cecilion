#include "Sandbox.h"


/**
 * Any node within a scene. This is synonymous with an entity.
 */



// https://manu343726.github.io/2019-07-14-reflections-on-user-defined-attributes/
// https://en.cppreference.com/w/cpp/keyword/reflexpr
class App : public Cecilion::Application {
public :
    App() {
        auto scene = Cecilion::Scene();


    }

    ~App() override {

    }

private:


};

Cecilion::Application *Cecilion::CreateApplication() {
    return new App();
}


bool Transform::operator==(const Transform &other) const {
    return false;
}

bool Transform::operator!=(const Transform &other) const {
    return false;
}

bool Transform::operator==(std::nullptr_t _) {
    return false;
}

bool Transform::operator!=(std::nullptr_t _) {
    return false;
}
