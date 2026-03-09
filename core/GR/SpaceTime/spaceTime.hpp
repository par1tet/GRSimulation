#pragma once

#include<diffgeomeng/classes/diff/Manifold.hpp>

class SpaceTime{
public:
    SpaceTime(Metric*);
    SpaceTime(Metric*, Embedding);

    Manifold* getManifold();
private:
    Manifold* manifold;
};