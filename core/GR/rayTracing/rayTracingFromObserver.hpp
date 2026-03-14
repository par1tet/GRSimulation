#pragma once

#include<vector>
#include<GR/rayTracing/ray.hpp>
#include<GR/observer.hpp>
#include<kinematics/body.hpp>

class RayTracingFromObserver{
public:
    RayTracingFromObserver(Observer*, std::vector<Body*> bodies, int width, int height, float FOV);

    void renderPixels();
    std::vector<Pixel> getPixelsBuffer();
private:
    std::vector<Pixel> pixelsBuffer;
    Observer* observer;
    std::vector<Body*> bodies;
    int width, height;
    float FOV;
};