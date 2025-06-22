#include<mesh/circleMesh.h>
#include<constans.h>

std::vector<float> getCircleMesh(float radius, int segments) {
    std::vector<float> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        vertices.push_back(radius / WIDTH * cos(angle)); // x
        vertices.push_back(radius / HEIGHT * sin(angle)); // y
        vertices.push_back(0.0f);               // z
    }

    return vertices;
}