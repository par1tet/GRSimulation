#include<kinematics/physicsEngine.hpp>
#include<iostream>

PhysicsEngine::PhysicsEngine(std::vector<Body*> bodies){
    this->bodies = bodies;
}

void PhysicsEngine::update(double dt){
    std::cout << "update" << std::endl;

    
}