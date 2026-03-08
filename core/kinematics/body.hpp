#include<mesh/sphereMesh.hpp>
#include<glm/glm.hpp>
#include<vector>
#include<diffgeomeng/utility/types.hpp>

#pragma once

class Body {
public:
    Body(State* state, glm::vec4 force, float radius, unsigned long long mass);

    State* getState();
    void setState(State* state);
    float getSelfTime();
    void setSelfTime(float time);
private:
    glm::vec4 force;
    State* state;
    float radius;
    unsigned long long mass; 
    float selftime;
};