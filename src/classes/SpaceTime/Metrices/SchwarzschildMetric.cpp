#include<core/classes/SpaceTime/Metrices/SchwarzschildMetric.hpp>

double SchwarzschildMetric00(const std::vector<double> &x,double mass){

}

double SchwarzschildMetric11(const std::vector<double> &x,double mass){

}
double SchwarzschildMetric22(const std::vector<double> &x,double mass){

}
double SchwarzschildMetric33(const std::vector<double> &x, double mass){

}

std::vector<std::function<double(const std::vector<double>&)>> SchwarzschildComponents(double mass){
    return {
        [mass](const std::vector<double> &x){return SchwarzschildMetric00(x, mass);}, 
        [mass](const std::vector<double> &x){return SchwarzschildMetric00(x, mass);}, 
        [mass](const std::vector<double> &x){return SchwarzschildMetric00(x, mass);}, 
        [mass](const std::vector<double> &x){return SchwarzschildMetric00(x, mass);}
    };
} 

SchwarzschildMetric::SchwarzschildMetric(double mass): Metric(SchwarzschildComponents(mass)) {
    this->mass = mass;
}