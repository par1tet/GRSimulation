#include<GR/SpaceTime/Embeddings/SchwarzschildEmbedding.hpp>
#include<cmath>
#include<constans.h>

double embeddingZ(double r, double mass){
    double rs = 2*G*mass/(c*c);
    return 2*sqrt(rs*(r-rs));
}



Embedding SchwarzchildEmbedding(double mass){
    return [mass](std::vector<double> x){
        int n = x.size();
        std::vector<double> x_dec(n, 0);

        double r = x[1];
        double phi = x[3];

        x_dec[1] = r * cos(phi);
        x_dec[2] = r * sin(phi);
        x_dec[3] = embeddingZ(r, mass);

        return x_dec;
    };
}