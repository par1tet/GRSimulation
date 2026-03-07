#include<core/classes/SpaceTime/spaceTime.hpp>
#include<core/classes/SpaceTime/metricFactory.hpp>

SpaceTime::SpaceTime(Metric* metric){
    this->manifold = new Manifold(metric);
}