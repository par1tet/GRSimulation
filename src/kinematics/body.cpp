#include<kinematics/body.hpp>
#include<iostream>
#include<constans.h>
#include<math.h>
#include<diffgeomeng/classes/diff/Manifold.hpp>

Body::Body(State *state, glm::vec4 force, float radius, unsigned long long mass){
    this->state = state;
    this->force = force;
    this->radius = radius;
    this->mass = mass;
    this->selftime = 0;
}

State* Body::getState(){
    return this->state;
}

void Body::setState(State* state){
    this->state = state;
}

void Body::setSelfTime(float newSelfTime){
    this->selftime = newSelfTime;
}

float Body::getSelfTime(){
    return this->selftime;
}