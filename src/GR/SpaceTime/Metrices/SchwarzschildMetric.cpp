#include<GR/SpaceTime/Metrices/SchwarzschildMetric.hpp>
#include<constans.h>
#include<cmath>

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

SchwarzschildMetric::SchwarzschildMetric(double mass): Metric(SchwarzschildComponents(mass)) {
    this->mass = mass;
}