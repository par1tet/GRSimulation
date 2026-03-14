#include<kinematics/physicsEngine.hpp>
#include<iostream>
#include<constans.h>
#include<diffgeomeng/classes/compute/rk4_realize.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>

PhysicsEngine::PhysicsEngine(std::vector<Body*> bodies, SpaceTime* spaceTime){
    this->bodies = bodies;
    this->time = 0;
    this->spaceTime = spaceTime;

    Manifold* manifold = this->spaceTime->getManifold();

    for (Body* body : bodies){
        State* state = body->getState();
        *state = manifold->normalizeVelocity(*state, 1);
        body->setState(state);
    }
}

// JJTODO СДЕЛАТЬ МАЛЕНЬКИЕ ПРИКОЛЬЧИКИ ДЛЯ НУЛЛ ГЕОДЕЗИКОВ ФОТОНЧИКАВА !! ^^
void PhysicsEngine::update(double dt, bool isUsingGeodesicRHS){
    Manifold* manifold = this->spaceTime->getManifold();
    Geodesic* geodesic = manifold->getGeodesic();
    Metric* metric = manifold->getMetric();
    double newTime = this->time + dt;

    GRMetric* grMetric = dynamic_cast<GRMetric*>(metric);

    if (!grMetric) {
        std::cerr << "Metric of SpaceTime is not a GRMetric!" << std::endl;
    }

    for (Body* body : bodies){
        State* state = body->getState();

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
                zero,
                false
            );
        }else{
            grMetric->computeIntegralParams(*state);
            *state = computeRK4(dtau, [grMetric](double t, State state) {
                return grMetric->MetricFirstIntegralRhs(t, state, 1, zero, false);
            }, *state, 0.002);
        }
        

        body->setState(state);
        body->setSelfTime(body->getSelfTime() + dtau);

    }

    this->time = newTime;
}