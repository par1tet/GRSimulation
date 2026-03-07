#pragma once

#include<diffgeomeng/classes/diff/Manifold.hpp>
#include<core/enums.h>

class SpaceTime{
public:
    SpaceTime(Metric*);

    Manifold* getManifold();
private:
    Manifold* manifold;
};