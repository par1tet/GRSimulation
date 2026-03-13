#include<GR/SpaceTime/Metrices/SchwarzschildMetric.hpp>
#include<constans.h>
#include<cmath>
#include<iostream>

// Metric signature: (+ - - -)

double SchwarzschildMetric00(const std::vector<double> &x,double mass){
    double rs = 2*G*mass/(c*c);
    return (1-rs/x[1]);
}

double SchwarzschildMetric11(const std::vector<double> &x,double mass){
    double rs = 2*G*mass/(c*c);

    return -(1/(1-rs/x[1]));
}
double SchwarzschildMetric22(const std::vector<double> &x,double mass){
    return -x[1]*x[1];
}
double SchwarzschildMetric33(const std::vector<double> &x, double mass){
    double s = sin(x[2]);
    return -x[1]*x[1]* s*s;
}

std::vector<std::function<double(const std::vector<double>&)>> SchwarzschildComponents(double mass){
    return {
        [mass](const std::vector<double> &x){return SchwarzschildMetric00(x, mass);}, 
        [mass](const std::vector<double> &x){return SchwarzschildMetric11(x, mass);}, 
        [mass](const std::vector<double> &x){return SchwarzschildMetric22(x, mass);}, 
        [mass](const std::vector<double> &x){return SchwarzschildMetric33(x, mass);}
    };
} 

SchwarzschildMetric::SchwarzschildMetric(double mass): GRMetric(SchwarzschildComponents(mass)) {
    this->params.mass = mass;
}

ParamSchwarz SchwarzschildMetric::getParams(){
    return this->params;
}

State SchwarzschildMetric::MetricFirstIntegralRhs(double time, State& initState, double kappa,
         std::function<std::vector<double>(std::vector<double>)> force, bool isLogging){
    double x_t = initState.x0[0], x_r = initState.x0[1], x_theta = initState.x0[2], x_phi = initState.x0[3];
    double u_t = initState.v0[0], u_r = initState.v0[1], u_theta = initState.v0[2], u_phi = initState.v0[3];

    double M = this->params.mass;
    double E = this->integrateParamsScharz.E;
    double L = this->integrateParamsScharz.L;

    double f = 1.0 - 2*M/x_r;
    double dVdr =
        (2*M/(x_r*x_r))*(kappa + L*L/(x_r*x_r))
        - f*(2*L*L/(x_r*x_r*x_r));
    double s = sin(x_theta);

    State newState{4};

    if(isLogging){
        std::cout << "<-----Positioin----->" << std::endl;

        for(int i = 0;i != initState.dimension;i++){
            std::cout << "coordinate " << i << " :" << initState.x0[i] << std::endl;
        }

        std::cout << "<-----Velocity----->" << std::endl;

        for(int i = 0;i != initState.dimension;i++){
            std::cout << "coordinate " << i << " :" << initState.v0[i] << std::endl;
        }
    }
 
    newState.x0[0] = E / f;        // dt/dτ
    newState.x0[1] = u_r;          // dr/dτ
    newState.x0[2] = u_theta;      // dθ/dτ
    newState.x0[3] = L/(x_r*x_r);  // dφ/dτ

    newState.v0[0] = -(2.0*M*E)/(x_r*x_r*f*f) * u_r;  // d²t/dτ²
    newState.v0[1] = -0.5 * dVdr;                     // d²r/dτ²
    newState.v0[2] = 0.0;                             // θ motion (plane)
    newState.v0[3] = -(2.0*L)/(x_r*x_r*x_r) * u_r;    // d²φ/dτ²

    return newState;

    return newState;
}

void SchwarzschildMetric::computeIntegralParams(State state){
    double x_t = state.x0[0], x_r = state.x0[1], x_theta = state.x0[2], x_phi = state.x0[3];
    double u_t = state.v0[0], u_r = state.v0[1], u_theta = state.v0[2], u_phi = state.v0[3];

    this->integrateParamsScharz.E = (1.0 - 2.0*this->params.mass/state.x0[1]) * state.v0[0]; 
    this->integrateParamsScharz.L = state.x0[1]*state.x0[1]*state.v0[3];

    if (x_theta < M_PI/2-1e-5 || x_theta > M_PI/2+1e-5){
        double s = sin(x_theta);
        this->integrateParamsScharz.L = sqrt(x_r*x_r*x_r*x_r*(u_theta*u_theta + s*s*u_phi*u_phi));
    }
}