#include<GR/rayTracing/ray.hpp>
#include<diffgeomeng/classes/compute/rk4_realize.hpp>
#include<diffgeomeng/classes/diff/Geodesic.hpp>

Ray::Ray(State<4>* state, Manifold<4>* manifold){
    this->state = state;

    *this->state = manifold->normalizeVelocity(*this->state, 0);
}

// TODO: do real sky texture
Pixel computeSkyRay(State<4>* state){
    return glm::vec3{0, 0.08, 0.09};
}

void Ray::integrateRay(double time, GRMetric<4>* grMetric, Manifold<4>* manifold, std::vector<Body<4>*> bodies, bool isUsingGeodesicRHS){
    double dt = 0.02;
    double dx = 0.02;
    double integratedTime = 0;

    // default value
    this->pixel = glm::vec3{0, 0.08, 0.09};
    
    grMetric->computeIntegralParams(*this->state);

    while (integratedTime <= time){
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
            *this->state = computeRK4<4>(dt, [grMetric](double t, State<4> state) {
                return grMetric->MetricFirstIntegralRhs(t, state, 0, zeroVectorField<4>(), false);
            }, *this->state, dx);
        }

        // TODO: <= 2*M fall into blackhole
        if(this->state->x0[1] <= 2){
            this->pixel = glm::vec3{0};

            return;
        }

        // TODO: do red bias effect
        for(Body<4>* body : bodies){
            Point<4> embBody = manifold->doEmbedding(body->getState()->x0);
            Point<4> embRay = manifold->doEmbedding(this->state->x0);

            glm::vec3 dir{embBody.x[1] - embRay.x[1], embBody.x[2] - embRay.x[2], embBody.x[3] - embRay.x[3]};
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
