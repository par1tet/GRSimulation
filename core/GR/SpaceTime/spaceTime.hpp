#pragma once

#include<diffgeomeng/classes/diff/Manifold.hpp>

template <size_t N>
class SpaceTime{
public:
    SpaceTime(Metric<N>*);
    SpaceTime(Metric<N>*, Embedding<N>);

    Manifold<N>* getManifold();
private:
    Manifold<N>* manifold;
};

#include<src/GR/SpaceTime/spaceTime.tpp>