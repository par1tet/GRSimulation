#include<GR/rayTracing/ray.hpp>
#include<diffgeomeng/classes/compute/rk4_realize.hpp>
#include<diffgeomeng/classes/diff/Geodesic.hpp>

Ray::Ray(State* state, Manifold* manifold){
    this->state = state;

    *state = manifold->normalizeVelocity(*state, 0);
}

// TODO: do real sky texture
Pixel computeSkyRay(State* state){
    return glm::vec3{0, 0.08, 0.09};
}

void Ray::integrateRay(double time, GRMetric* grMetric, Manifold* manifold, std::vector<Body*> bodies, bool isUsingGeodesicRHS){
    double dt = 0.02;
    double dx = 0.02;
    double integratedTime = 0;

    // default value
    this->pixel = glm::vec3{0, 0.08, 0.09};
    
    while (integratedTime <= time){
        if(isUsingGeodesicRHS){
            Geodesic* geodesic = new Geodesic(new ChristoffelSymbols(grMetric));

            *this->state = geodesic->computeGeodesicNextState(
                dt,
                *this->state,
                dx,
                zero,
                false
            );
        }else{
            grMetric->computeIntegralParams(*this->state);

            *this->state = computeRK4(dt, [grMetric](double t, State state) {
                return grMetric->MetricFirstIntegralRhs(t, state, 0, zero, false);
            }, *this->state, dx);
        }

        // TODO: <= 2*M fall into blackhole
        if(this->state->x0[1] <= 2){
            this->pixel = glm::vec3{0};

            return;
        }

        // TODO: do red bias effect
        for(Body* body : bodies){
            std::vector<double> embBody = manifold->doEmbedding(body->getState()->x0);
            std::vector<double> embRay = manifold->doEmbedding(this->state->x0);

            glm::vec3 dir{embBody[1] - embRay[1], embBody[2] - embRay[2], embBody[3] - embRay[3]};
            double r = body->getRadius();

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
