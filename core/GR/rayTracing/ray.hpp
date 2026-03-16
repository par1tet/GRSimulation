#pragma once

#include<glm/glm.hpp>
#include<diffgeomeng/utility/types.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>
#include<diffgeomeng/classes/diff/Manifold.hpp>
#include<kinematics/body.hpp>
#include <omp.h>

typedef glm::vec2 Pixel;

class Ray{
public:
    Ray(State<4>*, Manifold<4>* manifold);
    Pixel getPixel();
    void integrateRay(double time, GRMetric<4>* grMetric, Manifold<4>* manifold, int countBodies, const Body<4>* const* bodies, const Point<4>* embBodies, bool isUsingGeodesicRHS);

private:
    State<4>* state;
    Pixel pixel;
};