#pragma once

#include<glm/glm.hpp>
#include<diffgeomeng/utility/types.hpp>

typedef glm::vec3 Pixel;

class Ray{
public:
    Ray(State);
    Pixel getPixel();
    void integrateRay(double time);

private:
    State state;
};