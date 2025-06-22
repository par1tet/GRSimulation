#include<mesh/sphereMesh.hpp>
#include<glm/glm.hpp>
#include<vector>

#pragma once

class Body {
public:
    glm::vec3 position, speed, acceleration;
    float radius;

    Body(glm::vec3 position, glm::vec3 speed, glm::vec3 acceleration, float radius);

    void update(float dt, std::vector<Body*> bodies, int selfID);
};