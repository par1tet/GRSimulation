#include<mesh/sphereMesh.hpp>
#include<glm/glm.hpp>
#include<vector>

#pragma once

class Body {
public:
    Body(glm::vec4 position, glm::vec4 speed, glm::vec4 force, float radius, unsigned long long mass);
    Body(glm::vec3 position, glm::vec4 speed, glm::vec4 force, float radius, unsigned long long mass);

    void update(float dt, std::vector<Body*> bodies, int selfID);
    glm::vec4 getPosition();
private:
    glm::vec4 position, speed, force;
    float radius;
    unsigned long long mass; 
    float selftime;
};