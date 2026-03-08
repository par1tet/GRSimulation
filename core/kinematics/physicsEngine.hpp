#pragma once

#include<kinematics/body.hpp>
#include<GR/SpaceTime/spaceTime.hpp>

class PhysicsEngine{
public:
    PhysicsEngine(std::vector<Body*> bodies, SpaceTime* spaceTime);

    void update(double dt);
    float getSelfTimeOfBody(int i);
private:
    std::vector<Body*> bodies;
    SpaceTime* spaceTime;
    
    float time;
};