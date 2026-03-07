#include<kinematics/body.hpp>
#include<iostream>
#include<constans.h>
#include<math.h>
#include<diffgeomeng/classes/diff/Manifold.hpp>

Body::Body(glm::vec3 position, glm::vec4 speed, glm::vec4 force, float radius, unsigned long long mass){
    this->position = glm::vec4{0.f, position};
    this->speed = speed;
    this->force = force;
    this->radius = radius;
    this->mass = mass;
    this->selftime = 0;
}

Body::Body(glm::vec4 position, glm::vec4 speed, glm::vec4 force, float radius, unsigned long long mass){
    this->position = position;
    this->speed = speed;
    this->force = force;
    this->radius = radius;
    this->mass = mass;
    this->selftime = 0;
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

glm::vec4 Body::getPosition(){
    return this->position;
}