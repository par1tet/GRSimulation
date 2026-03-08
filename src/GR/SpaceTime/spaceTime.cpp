#include<GR/SpaceTime/spaceTime.hpp>

SpaceTime::SpaceTime(Metric* metric){
    this->manifold = new Manifold(metric);
}

Manifold* SpaceTime::getManifold(){
    return this->manifold;
}