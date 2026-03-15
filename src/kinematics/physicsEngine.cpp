#include<kinematics/physicsEngine.hpp>
#include<iostream>
#include<constans.h>
#include<diffgeomeng/classes/compute/rk4_realize.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>

template <size_t N>
PhysicsEngine<N>::PhysicsEngine(std::vector<Body<N>*> bodies, SpaceTime<N>* spaceTime){
    this->bodies = bodies;
    this->time = 0;
    this->spaceTime = spaceTime;

    Manifold<N>* manifold = this->spaceTime->getManifold();

    for (Body<N>* body : bodies){
        State<N>* state = body->getState();
        *state = manifold->normalizeVelocity(*state, 1);
        body->setState(state);
    }
}

template <size_t N>
// JJTODO СДЕЛАТЬ МАЛЕНЬКИЕ ПРИКОЛЬЧИКИ ДЛЯ НУЛЛ ГЕОДЕЗИКОВ ФОТОНЧИКАВА !! ^^
void PhysicsEngine<N>::update(double dt, bool isUsingGeodesicRHS){
    Manifold<N>* manifold = this->spaceTime->getManifold();
    Geodesic<N>* geodesic = manifold->getGeodesic();
    Metric<N>* metric = manifold->getMetric();
    double newTime = this->time + dt;

    GRMetric<N>* grMetric = dynamic_cast<GRMetric<N>*>(metric);

    if (!grMetric) {
        std::cerr << "Metric of SpaceTime is not a GRMetric!" << std::endl;
    }

    for (Body<N>* body : bodies){
        State<N>* state = body->getState();

        std::cout << metric->getInvariant(*state) << std::endl;

        double dtau = std::max(dt / state->v0[0], 1e-7);

        if(!true){
            for(int k=0;k<4;k++)
            for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
            {
                double g = manifold->getGeodesic()->getChristoffelSymbols()->computeChristoffelSybmbolsAtPoint(state->x0,k,i,j);
                if(abs(g) > 1e-6)
                    std::cout << "Γ_{" << k << i << j << "} = " << g << std::endl;
            }
        }

        double ar = 0;

        for(int a=0;a<4;a++)
        for(int b=0;b<4;b++){
            double g = manifold->getGeodesic()->getChristoffelSymbols()->computeChristoffelSybmbolsAtPoint(state->x0,1,a,b);
            ar -= g * state->v0[a] * state->v0[b];
        }

        printf("a_r = %f\n", ar);

        if (fabs(state->v0[0]) < 1e-12)
            throw "OBJECT MOVE'S SO SLOWLY IN SPACE--TIME!!!1";

        if(isUsingGeodesicRHS){
            *state = geodesic->computeGeodesicNextState(
                dtau,
                *state,
                0.002,
                zeroVectorField<4>(),
                false
            );
        }else{
            grMetric->computeIntegralParams(*state);
            *state = computeRK4(dtau, [grMetric](double t, State<N> state) {
                return grMetric->MetricFirstIntegralRhs(t, state, 1, zeroVectorField<4>(), false);
            }, *state, 0.002);
        }
        

        body->setState(state);
        body->setSelfTime(body->getSelfTime() + dtau);

    }

    this->time = newTime;
}