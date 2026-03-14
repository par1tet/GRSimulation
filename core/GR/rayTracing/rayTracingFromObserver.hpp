#pragma once

#include<vector>
#include<GR/rayTracing/ray.hpp>
#include<GR/observer.hpp>

class RayTracingFromObserver{
public:
    RayTracingFromObserver(Observer, int width, int height);

    void renderPixels();
    std::vector<Pixel> getPixelsBuffer();
private:
    std::vector<Pixel> pixelsBuffer;
    Observer* currentObserver;
};