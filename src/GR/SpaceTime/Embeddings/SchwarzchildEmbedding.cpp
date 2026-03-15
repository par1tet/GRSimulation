#include<GR/SpaceTime/Embeddings/SchwarzschildEmbedding.hpp>
#include<cmath>
#include<constans.h>

double embeddingZ(double r, double mass){
    double rs = 2*G*mass/(c*c);
    return 2*sqrt(rs*(r-rs));
}

Embedding<4> SchwarzchildEmbedding(double mass){
    return Embedding<4>([mass](Point<4> x){
        Point<4> x_dec;

        double r = x.x[1];
        double phi = x.x[3];

        x_dec.x[1] = r * cos(phi);
        x_dec.x[2] = r * sin(phi);
        x_dec.x[3] = embeddingZ(r, mass);

        return x_dec;
    });
}