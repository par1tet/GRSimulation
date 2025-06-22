#include<vector>
#include<cmath>
#include<string>
#include<vector>
#include<glm/glm.hpp>

#pragma once

std::vector<float> getCircleMesh(float radius, int segments);
std::vector<float> generateSphereVertices(float radius, int sectors, int stacks);