template <size_t N>
PhysicsEngine<N>::PhysicsEngine(const std::vector<Body<N>*>& bodies, SpaceTime<N>* spaceTime){
    this->spaceTime = spaceTime;
    this->bodies = bodies;

    for (Body<N>* body : bodies){
        State<N>* state = body->getState();
        std::cout << state->v0[0] << std::endl;

        auto g = this->spaceTime->getManifold()->getMetric()->getMatrixAtPoint(state->x0);

        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                std::cout << "g_" << i << j << " = " << g[i][j] << std::endl;
    }    



    for (Body<N>* body : this->bodies){
        State<N>* state = body->getState();

        std::cout << state->v0[0] << std::endl;

        *state = this->spaceTime->getManifold()->normalizeVelocity(*state, 1);

        std::cout << state->v0[0] << std::endl;

        body->setState(state);
    }    

    this->time = 0;
}

// JJTODO СДЕЛАТЬ МАЛЕНЬКИЕ ПРИКОЛЬЧИКИ ДЛЯ НУЛЛ ГЕОДЕЗИКОВ ФОТОНЧИКАВА !! ^^
template <size_t N>
void PhysicsEngine<N>::update(double dt, bool isUsingGeodesicRHS){
    Manifold<N>* manifold = this->spaceTime->getManifold();
    Geodesic<N>* geodesic = manifold->getGeodesic();
    Metric<N>* metric = manifold->getMetric();
    double newTime = this->time + dt;

    GRMetric<N>* grMetric = dynamic_cast<GRMetric<N>*>(metric);

    std::cout << grMetric <<std::endl;

    if (!grMetric) {
        std::cerr << "Metric of SpaceTime is not a GRMetric!" << std::endl;
    }

    for (Body<N>* body : this->bodies){
        State<N>* state = body->getState();

        std::cout << state->v0[0] << std::endl;

        std::cout << metric->getInvariant(*state) << std::endl;

        auto g = metric->getMatrixAtPoint(state->x0);
        std::cout << "g_00: " << g[0][0] << std::endl;


        std::cout << state->v0[0] << std::endl;

        std::cout << state->v0[1] << std::endl;
        std::cout << state->v0[2] << std::endl;
        std::cout << state->v0[3] << std::endl;

        double dtau = std::max(dt / state->v0[0], 1e-7);


        if(!true){
            for(int k=0;k<N;k++)
            for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
            {
                double g = manifold->getGeodesic()->getChristoffelSymbols()->computeChristoffelSybmbolsAtPoint(state->x0,k,i,j);
                if(abs(g) > 1e-6)
                    std::cout << "Γ_{" << k << i << j << "} = " << g << std::endl;
            }
        }

        double ar = 0;

        for(int a=0;a<N;a++)
        for(int b=0;b<N;b++){
            double g = manifold->getGeodesic()->getChristoffelSymbols()->computeChristoffelSybmbolsAtPoint(state->x0,1,a,b);
            ar -= g * state->v0[a] * state->v0[b];
        }

        printf("a_r = %f\n", ar);

        if (fabs(state->v0[0]) < 1e-12)
            throw "OBJECT MOVE'S SO SLOWLY IN SPACE--TIME!!!1";

        if(isUsingGeodesicRHS){
            *state = geodesic->computeGeodesicNextState (
                dtau,
                *state,
                0.002,
                zeroVectorField<4>(),
                false
            );
        }else{
            grMetric->computeIntegralParams(*state);
            *state = computeRK4<N>(dtau, [grMetric](double t, State<N> state) {
                return grMetric->MetricFirstIntegralRhs(t, state, 1, zeroVectorField<4>(), false);
            }, *state, 0.002);
        }
        

        body->setState(state);
        body->setSelfTime(body->getSelfTime() + dtau);

    }

    this->time = newTime;
}