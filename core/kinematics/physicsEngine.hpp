#pragma once

#include<kinematics/body.hpp>
#include<GR/SpaceTime/spaceTime.hpp>
#include<iostream>
#include<constans.h>
#include<diffgeomeng/classes/compute/rk4_realize.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>

template<size_t N>
class PhysicsEngine{
public:
    PhysicsEngine(const std::vector<Body<N>*>& bodies, SpaceTime<N>* spaceTime);

    void update(double dt, bool isUsingGeodesicRHS = true);
    float getSelfTimeOfBody(int i);
private:
    std::vector<Body<N>*> bodies;
    SpaceTime<N>* spaceTime;
    
    float time;
};

#include<src/kinematics/physicsEngine.tpp>