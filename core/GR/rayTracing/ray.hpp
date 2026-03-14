#pragma once

#include<glm/glm.hpp>
#include<diffgeomeng/utility/types.hpp>

typedef glm::vec3 Pixel;

class Ray{
public:
    Ray(State);
    Pixel getPixel();
    void IntegrateRay();

private:
    State state;
};