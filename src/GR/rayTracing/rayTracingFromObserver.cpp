#include<GR/rayTracing/rayTracingFromObserver.hpp>
#include<cmath>

RayTracingFromObserver::RayTracingFromObserver(Observer* obs, int width, int height, float FOV){
    this->observer = obs;
    this->width = width;
    this->height = height;
    this->FOV = FOV;
}

void RayTracingFromObserver::renderPixels(){
    Tetrad tetrad = this->observer->getTetrad();

    Manifold* manifold = this->observer->getManifold();
    std::vector<double> obsPos = (this->observer->getBody()->getState()->x0);

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

            Ray ray = Ray(State(rayStartPos, std::vector<double>{kVel[0], kVel[1], kVel[2], kVel[3]}, 4));

            ray.integrateRay(10);

            this->pixelsBuffer.push_back(ray.getPixel());
        }
    }
}

std::vector<Pixel> RayTracingFromObserver::getPixelsBuffer(){
    return this->pixelsBuffer;
}