
template <int width, int height>
RayTracingFromObserver<width, height>::RayTracingFromObserver(Observer* obs, std::vector<Body<4>*> bodies, float FOV){
    this->FOV = FOV;
    this->bodies = bodies;
    this->observer = obs;
    this->pixelsBuffer.fill({0,0,0}); 

    const double invW = 1.0 / width;
    const double invH = 1.0 / height;

    double s = tan(FOV/2);
    
    for(int py = 0;py < height;py++){
        for(int px = 0;px < width;px++){
            double u = (px + .5) * invW;
            double v = (py + .5) * invH;

            double x = 2*u - 1;
            double y = 1 - 2*v;

            x *= s;
            y *= s * height / width;

            double inv = 1.0 / sqrt(x*x + y*y + 1.0);

            glm::vec3 dirPhoton{
                x * inv,
                y * inv,
                -inv
            };

            this->photonDirs[px + py * width] = dirPhoton;
        }
    }
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

    int countBodies = this->bodies.size();

    Point<4>* embBodies = new Point<4>[countBodies];
    Body<4>** bodiesArr = new Body<4>*[countBodies];

    for(int i = 0;i != countBodies;i++){
        embBodies[i] = manifold->doEmbedding(this->bodies[i]->getState()->x0);
        bodiesArr[i] = bodies[i];
    }

    //#pragma omp parallel
    {
        State<4> state;
        Ray ray(&state, manifold);

        //#pragma omp for schedule(dynamic,   1)
        for(int py = 0;py < height;py++){
            for(int px = 0;px < width;px++){
                const glm::vec3& dirPhoton = this->photonDirs[px + py * width];

                glm::vec4 kVel;

                for (int mu = 0; mu < 4; mu++) {
                    kVel[mu] =
                        tetrad[0][mu] +
                        dirPhoton.x * tetrad[1][mu] +
                        dirPhoton.y * tetrad[2][mu] +
                        dirPhoton.z * tetrad[3][mu];
                }

                std::array<std::array<double, 4>, 4> g = manifold->getMetric()->getMatrixAtPoint(state.x0);
                Point<4> rayStartPos(obsPos);

                rayStartPos.x[1] += eps * kVel[1];
                rayStartPos.x[2] += eps * kVel[2];
                rayStartPos.x[3] += eps * kVel[3];

                state.x0 = rayStartPos.x;

                for(int k = 0;k != 4;k++){
                    for(int i = 0;i != 4;i++){
                        state.v0[k] += kVel[i] * g[k][i];
                    }
                }
                // double norm = 0;
                // for(int nu = 0;nu != 4;nu++){
                //     for(int mu = 0;mu != 4;mu++){
                //         norm += g[nu][mu] * state.v0[nu] * state.v0[mu];
                //     }
                // }
                // std::cout << "NORM KVEL: /// : " << norm << std::endl;

                // for(int a = 0; a < 4; a++){
                //     for(int b = 0; b < 4; b++){
                //         double val = 0;
                //         for(int mu = 0; mu < 4; mu++){
                //             for(int nu = 0; nu < 4; nu++){
                //                 val += g[mu][nu] * tetrad[a][mu] * tetrad[b][nu];
                //             }
                //         }
                //         std::cout << "g(e_" << a << ", e_" << b << ") = " << val << std::endl;
                //     }
                // }

                //std::cout << "start render ray at pixel: (" << px << ", " << py << ")" << std::endl;
                ray.integrateRay(15, grMetric, manifold, countBodies, bodiesArr, embBodies, false);
                

                this->pixelsBuffer[px + py * width] = (ray.getPixel());
            }
        }
    }

    delete[] embBodies;
    delete[] bodiesArr;
}

template <int width, int height>
std::array<Pixel, width*height> RayTracingFromObserver<width, height>::getPixelsBuffer(){
    return this->pixelsBuffer;
}