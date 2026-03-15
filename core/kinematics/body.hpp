#include<mesh/sphereMesh.hpp>
#include<glm/glm.hpp>
#include<vector>
#include<diffgeomeng/utility/types.hpp>

#pragma once

template <size_t N>
class Body {
public:
    Body(State<N>* state, glm::vec4 force, float radius, unsigned long long mass);

    State<N>* getState();
    void setState(State<N>* state);
    float getSelfTime();
    void setSelfTime(float time);
    float getRadius();

private:
    glm::vec4 force;
    State<N>* state;
    float radius;
    unsigned long long mass; 
    float selftime;
};