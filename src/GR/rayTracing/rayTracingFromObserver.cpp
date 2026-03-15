#include<GR/rayTracing/rayTracingFromObserver.hpp>
#include<cmath>
#include<iostream>

RayTracingFromObserver::RayTracingFromObserver(Observer* obs, std::vector<Body<4>*> bodies, int width, int height, float FOV){
    this->observer = obs;
    this->width = width;
    this->height = height;
    this->FOV = FOV;
    this->bodies = bodies;
}

void RayTracingFromObserver::renderPixels(){
    Tetrad tetrad = this->observer->getTetrad();

    Manifold<4>* manifold = this->observer->getManifold();
    Metric<4>* metric = manifold->getMetric();
    Point<4> obsPos = (this->observer->getBody()->getState()->x0);

    GRMetric<4>* grMetric = dynamic_cast<GRMetric<4>*>(metric);

    if(!grMetric){
        std::cerr << "Metric for manifold must be grMetric" << std::endl;
    }


    double eps = 1e-6;

    for(int px = 0;px != this->width;px++){
        for(int py = 0;py != this->height;py++){
            double u = (px + .5) / width;
            double v = (py + .5) / height;

            double x = 2*u - 1;
            double y = 1 - 2*v;

            double s = tan(FOV/2);

            x *= s;
            y *= s * this->height / this->width;

            glm::vec3 dirPhoton{x,y,-1};
            dirPhoton = glm::normalize(dirPhoton);

            glm::vec4 localPhotonDir{1.0, dirPhoton.x, dirPhoton.y, dirPhoton.z};

            glm::vec4 kVel = tetrad.e * localPhotonDir;

            Point<4> rayStartPos = obsPos;

            rayStartPos.x[1] += eps * kVel[1];
            rayStartPos.x[2] += eps * kVel[2];
            rayStartPos.x[3] += eps * kVel[3];

            Ray ray = Ray(new State<4>(rayStartPos.x, std::array<double, 4>{kVel[0], kVel[1], kVel[2], kVel[3]}), manifold);

            std::cout << "start render ray at pixel: (" << px << ", " << py << ")" << std::endl;
            ray.integrateRay(5, grMetric, manifold, bodies, false);

            this->pixelsBuffer.push_back(ray.getPixel());
        }
    }
}

std::vector<Pixel> RayTracingFromObserver::getPixelsBuffer(){
    return this->pixelsBuffer;
}