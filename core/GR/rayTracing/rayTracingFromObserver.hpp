#pragma once

#include<vector>
#include<GR/rayTracing/ray.hpp>
#include<GR/observer.hpp>

class RayTracingFromObserver{
public:
    RayTracingFromObserver(Observer*, int width, int height, float FOV);

    void renderPixels();
    std::vector<Pixel> getPixelsBuffer();
private:
    std::vector<Pixel> pixelsBuffer;
    Observer* observer;
    int width, height;
    float FOV;
};