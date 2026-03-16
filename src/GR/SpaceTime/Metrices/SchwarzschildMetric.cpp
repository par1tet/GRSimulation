#include<GR/SpaceTime/Metrices/SchwarzschildMetric.hpp>
#include<constans.h>
#include<cmath>
#include<iostream>

// Metric signature: (+ - - -)

double SchwarzschildMetric00(const Point<4> &x,double mass){
    double rs = 2*G*mass/(c*c);
    return (1-rs/x.x[1]);
}

double SchwarzschildMetric11(const Point<4> &x,double mass){
    double rs = 2*G*mass/(c*c);

    return -(1/(1-rs/x.x[1]));
}
double SchwarzschildMetric22(const Point<4> &x,double mass){
    return -x.x[1]*x.x[1];
}
double SchwarzschildMetric33(const Point<4> &x, double mass){
    double s = sin(x.x[2]);
    return -x.x[1]*x.x[1]* s*s;
}

ComponentsRowType<4> SchwarzschildComponents(double mass){
    return ComponentsRowType<4>({
        [mass](const Point<4> &x){return SchwarzschildMetric00(x, mass);}, 
        [mass](const Point<4> &x){return SchwarzschildMetric11(x, mass);}, 
        [mass](const Point<4> &x){return SchwarzschildMetric22(x, mass);}, 
        [mass](const Point<4> &x){return SchwarzschildMetric33(x, mass);}
    });
} 

SchwarzschildMetric::SchwarzschildMetric(double mass): GRMetric<4>(SchwarzschildComponents(mass)) {
    this->params.mass = mass;
}

ParamSchwarz SchwarzschildMetric::getParams(){
    return this->params;
}

inline State<4> SchwarzschildMetric::MetricFirstIntegralRhs(double time, State<4>& initState, double kappa,
         VectorField<4> force, bool isLogging){
    double x_t = initState.x0[0], x_r = initState.x0[1], x_theta = initState.x0[2], x_phi = initState.x0[3];
    double u_t = initState.v0[0], u_r = initState.v0[1], u_theta = initState.v0[2], u_phi = initState.v0[3];

    const double M = this->params.mass;
    const double E = this->integrateParamsScharz.E;
    const double L = this->integrateParamsScharz.L;

    const double inv_r = 1.0 / x_r;
    const double inv_r2 = inv_r * inv_r;
    const double inv_r3 = inv_r2 * inv_r;

    const double L2 = L * L;

    const double f = 1.0 - 2.0*M*inv_r;
    const double inv_f = 1.0 / f;
    const double inv_f2 = inv_f * inv_f;

    const double dVdr =
        (2.0*M*inv_r2)*(kappa + L2*inv_r2)
        - f*(2.0*L2*inv_r3);

    State<4> newState{};

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
 
    newState.x0[0] = E * inv_f;        // dt/dτ
    newState.x0[1] = u_r;          // dr/dτ
    newState.x0[2] = u_theta;      // dθ/dτ
    newState.x0[3] = L * inv_r2;  // dφ/dτ

    newState.v0[0] = -(2.0*M*E)*inv_r2*inv_f2 * u_r;  // d²t/dτ²
    newState.v0[1] = -0.5 * dVdr;                     // d²r/dτ²
    newState.v0[2] = 0.0;                             // θ motion (plane)
    newState.v0[3] = -(2.0*L)*(inv_r3) * u_r;    // d²φ/dτ²

    return newState;
}

void SchwarzschildMetric::computeIntegralParams(State<4> state){
    double x_t = state.x0[0], x_r = state.x0[1], x_theta = state.x0[2], x_phi = state.x0[3];
    double u_t = state.v0[0], u_r = state.v0[1], u_theta = state.v0[2], u_phi = state.v0[3];

    this->integrateParamsScharz.E = (1.0 - 2.0*this->params.mass/state.x0[1]) * state.v0[0]; 
    this->integrateParamsScharz.L = state.x0[1]*state.x0[1]*state.v0[3];

    if (x_theta < M_PI/2-1e-5 || x_theta > M_PI/2+1e-5){
        double s = sin(x_theta);
        this->integrateParamsScharz.L = sqrt(x_r*x_r*x_r*x_r*(u_theta*u_theta + s*s*u_phi*u_phi));
    }
}