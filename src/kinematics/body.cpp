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

glm::vec4 Body::getPosition(){
    return this->position;
}