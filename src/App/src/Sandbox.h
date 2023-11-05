#pragma once

#include <iostream>
#include <utility>
#include <bitset>
#include <vector>
#include <Utils/Sparse_set.h>
#include <Utils/Type.h>

#define API_OPENGL

#include <Cecilion.h>
#include <yaml-cpp/yaml.h>





class BehaviourScript {
public:
    BehaviourScript(const BehaviourScript&) = delete;
    BehaviourScript& operator=(const BehaviourScript&) = delete;

    virtual void start(){}
    virtual void active(){}
    virtual void inactive(){}
    virtual void update(){}
    virtual void destroy(){}
};



class Transform: BehaviourScript {

    bool operator==(const Transform& other) const;
    bool operator!=(const Transform& other) const;
    bool operator==(std::nullptr_t _);
    bool operator!=(std::nullptr_t _);

private:


};