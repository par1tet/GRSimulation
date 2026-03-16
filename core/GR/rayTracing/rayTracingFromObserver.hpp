#pragma once

#include<vector>
#include<GR/rayTracing/ray.hpp>
#include<GR/observer.hpp>
#include<kinematics/body.hpp>
#include<cmath>
#include<iostream>

template <int width, int height>
class RayTracingFromObserver{
public:
    RayTracingFromObserver(Observer*, std::vector<Body<4>*> bodies, float FOV);

    void renderPixels();
    std::array<Pixel, width*height> getPixelsBuffer();
private:
    std::array<Pixel, width*height> pixelsBuffer;
    Observer* observer;
    std::vector<Body<4>*> bodies;
    float FOV;
};

#include<src/GR/rayTracing/rayTracingFromObserver.tpp>