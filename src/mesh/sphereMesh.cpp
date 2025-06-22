#include<mesh/sphereMesh.hpp>
#include<constans.h>

std::vector<float> getCircleMesh(float radius, int segments) {
    std::vector<float> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        vertices.push_back(radius / WIDTH * cos(angle));
        vertices.push_back(radius / HEIGHT * sin(angle));
        vertices.push_back(0.0f);
    }

    return vertices;
}

std::vector<float> generateSphereVertices(float radius, int sectors, int stacks) {
    std::vector<float> vertices;
    const float PI = 3.14159265358f;
    
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2 - i * PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        
        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * PI / sectors;
            
            vertices.push_back(xy * cosf(sectorAngle));
            vertices.push_back(xy * sinf(sectorAngle));
            vertices.push_back(z);
            
            float nextStackAngle = PI / 2 - (i + 1) * PI / stacks;
            float nextXY = radius * cosf(nextStackAngle);
            float nextZ = radius * sinf(nextStackAngle);
            vertices.push_back(nextXY * cosf(sectorAngle));
            vertices.push_back(nextXY * sinf(sectorAngle));
            vertices.push_back(nextZ);
        }
    }
    return vertices;
}