#include<kinematics/physicsEngine.hpp>
#include<iostream>
#include<constans.h>

PhysicsEngine::PhysicsEngine(std::vector<Body*> bodies, SpaceTime* spaceTime){
    this->bodies = bodies;
    this->time = 0;
    this->spaceTime = spaceTime;
}

// JJTODO СДЕЛАТЬ МАЛЕНЬКИЕ ПРИКОЛЬЧИКИ ДЛЯ НУЛЛ ГЕОДЕЗИКОВ ФОТОНЧИКАВА !! ^^
void PhysicsEngine::update(double dt){
    std::cout << "update" << std::endl;

    Manifold* manifold = this->spaceTime->getManifold();
    Geodesic* geodesic = manifold->getGeodesic();
    double newTime = this->time + dt;

    for (Body* body : bodies){
        State* state = body->getState();


        std::cout << manifold->getMetric()->getInvariant(*state) << std::endl;

        double dtau = std::min(dt / state->v0[0], 0.001);;

        if (fabs(state->v0[0]) < 1e-12)
            throw "OBJECT MOVE'S SO SLOWLY IN SPACE--TIME!!!1";

        *state = geodesic->computeGeodesicNextState(
            dtau,
            *state
        );

        body->setState(state);
        body->setSelfTime(body->getSelfTime() + dtau);

        *state = manifold->normalizeVelocity(*state);
    }

    this->time = newTime;
}