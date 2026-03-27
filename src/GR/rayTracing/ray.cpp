#include<GR/rayTracing/ray.hpp>
#include<diffgeomeng/classes/compute/rk4_realize.hpp>
#include<diffgeomeng/classes/diff/Geodesic.hpp>
#include<diffgeomeng/classes/compute/yoshida4_realize.hpp>
#include<iostream>

Ray::Ray(State<4>* state, Manifold<4>* manifold){
    this->state = state;

    *this->state = manifold->normalizeVelocity(*this->state, 0);
}

// TODO: do real sky texture
Pixel computeSkyRay(State<4>* state){

    //return {1, 1.002, 1.007};
    return {0, 0.002, 1.007};
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
            const_cast<State<4>&>(s),
            0,
            zeroVectorField<4>(),
            false
        );
    }
};

// struct SchwarzschildHamiltonian : Hamiltonian<4> {
//     inline Vector<4> dHdp(Vector<4> x, Vector<4> p) const override {
//         Vector<4> newP{};

//         newP[0] = (-p[0])/(1-2/x[1]);
//         newP[1] = (1-2/x[1]) * p[1];
//         newP[2] = (p[2])/(x[1]*x[1]);
//         newP[3] = (p[3])/(x[1]*x[1]*sin(x[2])*sin(x[2]));

//         return newP;
//     }

//     inline Vector<4> dHdx(Vector<4> x, Vector<4> p) const override {
//         Vector<4> newX{};

//         newX[0] = 0;
//         newX[1] = 0.5 * (
//             (1/(x[1]*x[1]*(1-2/x[1])*(1-2/x[1]))) * p[0] * p[0]
//             + (p[1]*p[1])/(x[1]*x[1])
//             - (p[2]*p[2])/(x[1]*x[1]*x[1])
//             - (p[3]*p[3])/(x[1]*x[1]*x[1]*sin(x[2])*sin(x[2]))
//         );
//         newX[2] = -((cos(x[2]))/(x[1]*x[1]*sin(x[2])*sin(x[2])*sin(x[2]))) * p[3]*p[3];
//         newX[3] = 0;

//         return newX;
//     }
// };

struct SchwarzschildHamiltonian : Hamiltonian<4> {

    double M;

    inline Vector<4> dHdp(Vector<4> x, Vector<4> p) const override {
        Vector<4> out{};

        double r = x[1];
        double th = x[2];

        out[0] = -p[0] / (1 - 2*M/r);
        out[1] = (1 - 2*M/r) * p[1];
        out[2] = p[2] / (r*r);
        out[3] = p[3] / (r*r * sin(th)*sin(th));

        return out;
    }

    inline Vector<4> dHdx(Vector<4> x, Vector<4> p) const override {
        Vector<4> out{};

        double r = x[1];
        double th = x[2];

        out[0] = 0;

        out[1] =
            (M / (r*r * pow(1 - 2*M/r, 2))) * p[0]*p[0]
          + (M / (r*r)) * p[1]*p[1]
          - (1.0 / (r*r*r)) * p[2]*p[2]
          - (1.0 / (r*r*r * sin(th)*sin(th))) * p[3]*p[3];

        out[2] =
            -cos(th) / (r*r * pow(sin(th), 3))
            * p[3]*p[3];

        out[3] = 0;

        return out;
    }
};

void Ray::integrateRay(double time, GRMetric<4>* grMetric, Manifold<4>* manifold, int countBodies, const Body<4>* const* bodies, const Point<4>* embBodies, bool isUsingGeodesicRHS){
    *this->state = manifold->normalizeVelocity(*(this->state), 0);
    double base_dt = 0.002;
    double base_dx = 0.002;
    double integratedTime = 0;

    // default value
    this->pixel = {0, 0.08, 0.09};
    
    grMetric->computeIntegralParams(*this->state);
    double r_sky = 40;
    int steps = 0;

    MetricRhsFirstIntegralFunctor rhs(grMetric);

    while (integratedTime <= time){
        steps++;
        double r = this->state->x0[1];
        double dt = base_dt * std::min(1.0, r / 10.0);
        dt = std::max(dt, 0.00005);
        double dx = base_dx;

        if(steps >= 20000){
            std::cout << "piza" << std::endl;
            return;
        }

        std::array<std::array<double, 4>, 4> g = manifold->getMetric()->getMatrixAtPoint(state->x0);

        // TODO: <= 2*M fall into blackhole
        if(r <= 2){
            this->pixel = {0,1,0};
            std::cout << "BLACK HOUL" << std::endl;
            std::cout << "BLACK HOUL" << std::endl;
            std::cout << "BLACK HOUL" << std::endl;
            std::cout << "BLACK HOUL" << std::endl;
            std::cout << "BLACK HOUL" << std::endl;
            std::cout << "BLACK HOUL" << std::endl;
            std::cout << "BLACK HOUL" << std::endl;

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
            SchwarzschildHamiltonian hamSystem;
            //*this->state = computeRK4<4>(dt, rhs, *this->state, dx);
            *this->state = computeVerlet(&hamSystem, *this->state, dt);
        }

        double norm = 0;

        for(int nu = 0;nu != 4;nu++){
            for(int mu = 0;mu != 4;mu++){
                norm += g[nu][mu] * state->v0[nu] * state->v0[mu];
            }
        }

        //*this->state = manifold->normalizeVelocity(*(this->state), 0);

        //if(abs(norm) >= 0.0001){
            std::cout << "-----------------ALARM-----------------" << std::endl;
            std::cout << "WRONG NORMALIZE FOR RAY:: " << norm << std::endl;
            std::cout << "-----------------ALARM-----------------" << std::endl;
        //}

        // TODO: do red bias effect
        for(int i = 0;i != countBodies;i++){
            //Point<4> embBody = embBodies[i];
            //Point<4> embRay = manifold->doEmbedding(this->state->x0);

            //glm::vec3 dir{embBody.x[1] - embRay.x[1], embBody.x[2] - embRay.x[2], embBody.x[3] - embRay.x[3]};
            const double rBody = bodies[i]->getRadius();

            const double r = state->x0[1];
            double theta = state->x0[2];

            double dr = r - bodies[i]->getState()->x0[1];
            double dtheta = theta - bodies[i]->getState()->x0[2];
            double dphi = state->x0[3] - bodies[i]->getState()->x0[3];

            double dist2 =
                dr*dr +
                (r*r) * dtheta*dtheta +
                (r*r * sin(theta)*sin(theta)) * dphi*dphi;

            if(dist2 <= rBody*rBody){
                this->pixel = {1.f, 0.f, 0.f};

                return;
            }
        }

        integratedTime += dt;
    }
    std::cout << "R OF THE RAY: " << state->x0[1] << std::endl;
    this->pixel = computeSkyRay(this->state);
}

Pixel Ray::getPixel(){
    return this->pixel;
}
