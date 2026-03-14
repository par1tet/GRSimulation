#pragma once

#include<glm/glm.hpp>
#include<diffgeomeng/utility/types.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>
#include<diffgeomeng/classes/diff/Manifold.hpp>
#include<kinematics/body.hpp>

typedef glm::vec3 Pixel;

class Ray{
public:
    Ray(State*, Manifold* manifold);
    Pixel getPixel();
    void integrateRay(double time, GRMetric* grMetric, Manifold* manifold, std::vector<Body*> bodies, bool isUsingGeodesicRHS = true);

private:
    State* state;
    Pixel pixel;
};