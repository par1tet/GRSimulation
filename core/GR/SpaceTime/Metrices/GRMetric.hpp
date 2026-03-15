#pragma once

#include<diffgeomeng/classes/diff/Metric.hpp>
#include<GR/types.h>

template <size_t N>
class GRMetric : public Metric<N> {
public:
    GRMetric(Components<N> components): Metric<N>(components){};
    GRMetric(ComponentsRowType<N> components): Metric<N>(components){};

    virtual State<N> MetricFirstIntegralRhs(double time, State<N>& initState, double kappa,
         VectorField<N> force = zeroVectorField<N>(), bool isLogging = false){};

    virtual void computeIntegralParams(State<N>){};
};