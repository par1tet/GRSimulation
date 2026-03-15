#pragma once

#include<kinematics/body.hpp>
#include<GR/SpaceTime/spaceTime.hpp>

template<size_t N>
class PhysicsEngine{
public:
    PhysicsEngine(std::vector<Body<N>*> bodies, SpaceTime<N>* spaceTime);

    void update(double dt, bool isUsingGeodesicRHS = true);
    float getSelfTimeOfBody(int i);
private:
    std::vector<Body<N>*> bodies;
    SpaceTime<N>* spaceTime;
    
    float time;
};