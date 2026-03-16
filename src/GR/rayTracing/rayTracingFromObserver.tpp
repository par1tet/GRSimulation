template <int width, int height>
RayTracingFromObserver<width, height>::RayTracingFromObserver(Observer* obs, std::vector<Body<4>*> bodies, float FOV){
    this->FOV = FOV;
    this->bodies = bodies;
    this->observer = obs;
    this->pixelsBuffer.fill(glm::vec3{0}); 
}

template <int width, int height>
void RayTracingFromObserver<width, height>::renderPixels(){
    Tetrad tetrad = this->observer->getTetrad();

    Manifold<4>* manifold = this->observer->getManifold();
    Metric<4>* metric = manifold->getMetric();
    Point<4> obsPos = (this->observer->getBody()->getState()->x0);

    GRMetric<4>* grMetric = static_cast<GRMetric<4>*>(metric);

    if(!grMetric){
        std::cerr << "Metric for manifold must be grMetric" << std::endl;
    }

    double eps = 1e-6;

    double s = tan(FOV/2);
    int countBodies = this->bodies.size();

    Point<4>* embBodies = new Point<4>[countBodies];
    Body<4>** bodiesArr = new Body<4>*[countBodies];

    for(int i = 0;i != countBodies;i++){
        embBodies[i] = manifold->doEmbedding(this->bodies[i]->getState()->x0);
        bodiesArr[i] = bodies[i];
    }
    
    #pragma omp parallel for schedule(dynamic)
    for(int py = 0;py != width;py++){
        for(int px = 0;px != height;px++){
            double u = (px + .5) / width;
            double v = (py + .5) / height;

            double x = 2*u - 1;
            double y = 1 - 2*v;

            x *= s;
            y *= s * height / width;

            glm::vec3 dirPhoton{x,y,-1};
            dirPhoton = glm::normalize(dirPhoton);

            glm::vec4 localPhotonDir{1.0, dirPhoton.x, dirPhoton.y, dirPhoton.z};

            glm::vec4 kVel = tetrad.e * localPhotonDir;

            Point<4> rayStartPos = obsPos;

            rayStartPos.x[1] += eps * kVel[1];
            rayStartPos.x[2] += eps * kVel[2];
            rayStartPos.x[3] += eps * kVel[3];

            State<4> state(rayStartPos.x, std::array<double, 4>{kVel[0], kVel[1], kVel[2], kVel[3]});
            Ray ray(&state, manifold);

            //std::cout << "start render ray at pixel: (" << px << ", " << py << ")" << std::endl;
            ray.integrateRay(5, grMetric, manifold, countBodies, bodiesArr, embBodies, false);

            this->pixelsBuffer[px + py * width] = (ray.getPixel());
        }
    }

    delete[] embBodies;
    delete[] bodiesArr;
}

template <int width, int height>
std::array<Pixel, width*height> RayTracingFromObserver<width, height>::getPixelsBuffer(){
    return this->pixelsBuffer;
}