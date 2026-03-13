#pragma once

#include<GR/SpaceTime/Metrices/GRMetric.hpp>

struct ParamSchwarz {
    double mass;
};
struct IntegrateParamsSchwarz {
    double E;
    double L;
};

class SchwarzschildMetric : public GRMetric {
public:
    SchwarzschildMetric(double mass);

    ParamSchwarz getParams();

    State MetricFirstIntegralRhs(double time, State& initState, double kappa,
         std::function<std::vector<double>(std::vector<double>)> force = zero, bool isLogging = false);

    void computeIntegralParams(State);

    void updateParams(State);
private:
    ParamSchwarz params;
    IntegrateParamsSchwarz integrateParamsScharz;
};