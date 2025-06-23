#include<kinematics/body.hpp>
#include<iostream>
#include<constans.h>
#include<math.h>

Body::Body(glm::vec3 position, glm::vec3 speed, glm::vec3 acceleration, float radius, unsigned long long mass){
    this->position = position;
    this->speed = speed;
    this->acceleration = acceleration;
    this->radius = radius;
    this->mass = mass;
}

void Body::update(float dt, std::vector<Body*> bodies, int selfID){
    glm::vec3 newAcceleration{0.f};

    for(int i = 0;i != bodies.size();i++){
        if(i == selfID) continue;

        glm::vec3 direction = bodies[i]->position - this->position;

        float dist = glm::length(direction);

        float gravityForce = (G*bodies[i]->mass)/(dist*dist*dist);

        glm::vec3 gA = direction * gravityForce;

        newAcceleration+=gA;

        if(dist < bodies[i]->radius + this->radius){
            this->speed *= -1;
        }
    }

    this->acceleration = newAcceleration;

    this->speed += this->acceleration * dt;
    this->position += this->speed * dt;
}