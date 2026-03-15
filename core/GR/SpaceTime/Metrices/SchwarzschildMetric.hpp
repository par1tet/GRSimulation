#pragma once

#include<GR/SpaceTime/Metrices/GRMetric.hpp>
#include<diffgeomeng/utility/types.hpp>

struct ParamSchwarz {
    double mass;
};
struct IntegrateParamsSchwarz {
    double E;
    double L;
};

class SchwarzschildMetric : public GRMetric<4> {
public:
    SchwarzschildMetric(double mass);

    ParamSchwarz getParams();

    State<4> MetricFirstIntegralRhs(double time, State<4>& initState, double kappa,
         VectorField<4> force = zeroVectorField<4>(), bool isLogging = false);

    void computeIntegralParams(State<4>);

    void updateParams(State<4>);
private:
    ParamSchwarz params;
    IntegrateParamsSchwarz integrateParamsScharz;
};