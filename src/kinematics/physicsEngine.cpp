#include<kinematics/physicsEngine.hpp>
#include<iostream>
#include<constans.h>

PhysicsEngine::PhysicsEngine(std::vector<Body*> bodies, SpaceTime* spaceTime){
    this->bodies = bodies;
    this->time = 0;
    this->spaceTime = spaceTime;
}

void PhysicsEngine::update(double dt){
    std::cout << "update" << std::endl;

    Manifold* manifold = this->spaceTime->getManifold();
    Geodesic* geodesic = manifold->getGeodesic();
    double newTime = this->time + dt;

    for(int i = 0;i != this->bodies.size();i++){
        Body* currentBody = this->bodies[i];
        State* state = currentBody->getState();

        *state = manifold->normalizeVelocity(*state);
        
        float dtau = dt / state->v0[0];

        *state = geodesic->computeGeodesicNextState(
            dtau,
            *state,
            0.002
        );

        currentBody->setState(state);
        currentBody->setSelfTime(currentBody->getSelfTime() + dtau);
    }

    this->time = newTime;
}