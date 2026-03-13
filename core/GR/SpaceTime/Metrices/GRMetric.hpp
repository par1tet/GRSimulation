#pragma once

#include<diffgeomeng/classes/diff/Metric.hpp>

class GRMetric : public Metric {
public:
    GRMetric(Components components): Metric(components){};
    GRMetric(std::vector<std::function<double(const std::vector<double>&)>> components): Metric(components){};

    virtual State MetricFirstIntegralRhs(double time, State& initState, double kappa,
         std::function<std::vector<double>(std::vector<double>)> force = zero, bool isLogging = false){};

    virtual void computeIntegralParams(State){};
};