#include<kinematics/body.hpp>
#include<iostream>

Body::Body(glm::vec3 position, glm::vec3 speed, glm::vec3 acceleration, float radius){
    this->position = position;
    this->speed = speed;
    this->acceleration = acceleration;
}

void Body::update(float dt, std::vector<Body*> bodies, int selfID){
    this->position += this->speed * dt;
    this->speed += this->acceleration * dt;
}