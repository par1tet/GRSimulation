#include<GR/SpaceTime/spaceTime.hpp>

template<size_t N>
SpaceTime<N>::SpaceTime(Metric<N>* metric){
    this->manifold = new Manifold<N>(metric);
}

template<size_t N>
SpaceTime<N>::SpaceTime(Metric<N>* metric, Embedding<N> emb){
    this->manifold = new Manifold<N>(metric, emb);
}

template<size_t N>
Manifold<N>* SpaceTime<N>::getManifold(){
    return this->manifold;
}