#include<mesh/sphereMesh.hpp>
#include<glm/glm.hpp>
#include<vector>
#include<diffgeomeng/utility/types.hpp>
#include<iostream>
#include<constans.h>
#include<math.h>
#include<diffgeomeng/classes/diff/Manifold.hpp>
#include<diffgeomeng/utility/functions.hpp>

#pragma once

template <size_t N>
class Body {
public:
    Body(State<N>* state, glm::vec4 force, float radius, unsigned long long mass);

    State<N>* getState();
    void setState(State<N>* state);
    float getSelfTime();
    void setSelfTime(float time);
    float getRadius() const;

private:
    glm::vec4 force;
    State<N>* state;
    float radius;
    unsigned long long mass; 
    float selftime;
};

#include<src/kinematics/body.tpp>