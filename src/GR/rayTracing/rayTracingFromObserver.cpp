#include<GR/rayTracing/rayTracingFromObserver.hpp>
#include<cmath>
#include<iostream>

RayTracingFromObserver::RayTracingFromObserver(Observer* obs, std::vector<Body*> bodies, int width, int height, float FOV){
    this->observer = obs;
    this->width = width;
    this->height = height;
    this->FOV = FOV;
    this->bodies = bodies;
}

void RayTracingFromObserver::renderPixels(){
    Tetrad tetrad = this->observer->getTetrad();

    Manifold* manifold = this->observer->getManifold();
    Metric* metric = manifold->getMetric();
    std::vector<double> obsPos = (this->observer->getBody()->getState()->x0);

    GRMetric* grMetric = dynamic_cast<GRMetric*>(metric);

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

            std::vector<double> rayStartPos = obsPos;

            rayStartPos[1] += eps * kVel[1];
            rayStartPos[2] += eps * kVel[2];
            rayStartPos[3] += eps * kVel[3];

            Ray ray = Ray(new State(rayStartPos, std::vector<double>{kVel[0], kVel[1], kVel[2], kVel[3]}, 4), manifold);

            std::cout << "start render ray at pixel: (" << px << ", " << py << ")" << std::endl;
            ray.integrateRay(5, grMetric, manifold, bodies, false);

            this->pixelsBuffer.push_back(ray.getPixel());
        }
    }
}

std::vector<Pixel> RayTracingFromObserver::getPixelsBuffer(){
    return this->pixelsBuffer;
}