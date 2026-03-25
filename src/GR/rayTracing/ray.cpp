#include<GR/rayTracing/ray.hpp>
#include<diffgeomeng/classes/compute/rk4_realize.hpp>
#include<diffgeomeng/classes/diff/Geodesic.hpp>
#include<iostream>

Ray::Ray(State<4>* state, Manifold<4>* manifold){
    this->state = state;

    *this->state = manifold->normalizeVelocity(*this->state, 0);
}

// TODO: do real sky texture
Pixel computeSkyRay(State<4>* state){
    return glm::vec3{0, 0.08, 0.09};
}

struct MetricRhsFirstIntegralFunctor
{
    GRMetric<4>* metric;

    MetricRhsFirstIntegralFunctor(GRMetric<4>* m)
        : metric(m) {}

    inline State<4> operator()(double t, const State<4>& s) const
    {
        return metric->MetricFirstIntegralRhs(
            t,
            const_cast<State<4>&>(s),   // если не поменяешь сигнатуру
            0,
            zeroVectorField<4>(),
            false
        );
    }
};

void Ray::integrateRay(double time, GRMetric<4>* grMetric, Manifold<4>* manifold, int countBodies, const Body<4>* const* bodies, const Point<4>* embBodies, bool isUsingGeodesicRHS){
    double base_dt = 0.02;
    double base_dx = 0.02;
    double integratedTime = 0;

    // default value
    this->pixel = glm::vec3{0, 0.08, 0.09};
    
    grMetric->computeIntegralParams(*this->state);
    double r_sky = 100;
    int steps = 0;

    MetricRhsFirstIntegralFunctor rhs(grMetric);

    while (integratedTime <= time){
        steps++;
        double r = this->state->x0[1];
        double dt = base_dt * std::max(1.0, r/10.0);;
        double dx = base_dx * std::max(1.0, r/10.0);;

        if(steps >= 2000){
            return;
        }

        // TODO: <= 2*M fall into blackhole
        if(r <= 2){
            this->pixel = glm::vec3{0};

            return;
        }

        if(r >= r_sky){
            this->pixel = computeSkyRay(this->state);

            return;
        }

        if(isUsingGeodesicRHS){
            Geodesic<4>* geodesic = new Geodesic<4>(new ChristoffelSymbols<4>(grMetric));

            *this->state = geodesic->computeGeodesicNextState(
                dt,
                *this->state,
                dx,
                zeroVectorField<4>(),
                false
            );
        }else{
            *this->state = computeRK4<4>(dt, rhs, *this->state, dx);
        }

        // TODO: do red bias effect
        for(int i = 0;i != countBodies;i++){
            Point<4> embBody = embBodies[i];
            Point<4> embRay = manifold->doEmbedding(this->state->x0);

            glm::vec3 dir{embBody.x[1] - embRay.x[1], embBody.x[2] - embRay.x[2], embBody.x[3] - embRay.x[3]};
            double r = bodies[i]->getRadius();

            if(glm::dot(dir, dir) <= r*r){
                this->pixel = glm::vec3{1.f, 0.f, 0.f};

                return;
            }
        }

        integratedTime += dt;
    }

    this->pixel = computeSkyRay(this->state);
}

Pixel Ray::getPixel(){
    return this->pixel;
}
