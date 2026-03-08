#pragma once

#include<diffgeomeng/classes/diff/Manifold.hpp>

class SpaceTime{
public:
    SpaceTime(Metric*);

    Manifold* getManifold();
private:
    Manifold* manifold;
};