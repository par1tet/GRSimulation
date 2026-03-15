#include<kinematics/body.hpp>
#include<iostream>
#include<constans.h>
#include<math.h>
#include<diffgeomeng/classes/diff/Manifold.hpp>
#include<diffgeomeng/utility/functions.hpp>

template <size_t N>
Body<N>::Body(State<N> *state, glm::vec4 force, float radius, unsigned long long mass){
    checkCorrectState(*state);

    this->state = state;
    this->force = force;
    this->radius = radius;
    this->mass = mass;
    this->selftime = 0;
}

template <size_t N>
State<N>* Body<N>::getState(){
    return this->state;
}

template <size_t N>
void Body<N>::setState(State<N>* state){
    this->state = state;
}

template <size_t N>
void Body<N>::setSelfTime(float newSelfTime){
    this->selftime = newSelfTime;
}

template <size_t N>
float Body<N>::getSelfTime(){
    return this->selftime;
}

template <size_t N>
float Body<N>::getRadius(){
    return this->radius;
}