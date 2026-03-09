#include<GR/SpaceTime/spaceTime.hpp>

SpaceTime::SpaceTime(Metric* metric){
    this->manifold = new Manifold(metric);
}

SpaceTime::SpaceTime(Metric* metric, Embedding emb){
    this->manifold = new Manifold(metric, emb);
}

Manifold* SpaceTime::getManifold(){
    return this->manifold;
}