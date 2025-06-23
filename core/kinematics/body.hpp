#include<mesh/sphereMesh.hpp>
#include<glm/glm.hpp>
#include<vector>

#pragma once

class Body {
public:
    glm::vec3 position, speed, acceleration;
    float radius;
    unsigned long long mass; 

    Body(glm::vec3 position, glm::vec3 speed, glm::vec3 acceleration, float radius, unsigned long long mass);

    void update(float dt, std::vector<Body*> bodies, int selfID);
};