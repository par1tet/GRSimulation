#include<kinematics/body.hpp>

class PhysicsEngine{
public:
    PhysicsEngine(std::vector<Body*> bodies);

    void update(double dt);
private:
    std::vector<Body*> bodies;
};