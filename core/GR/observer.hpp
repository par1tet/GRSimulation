#pragma once

#include<diffgeomeng/utility/types.hpp>
#include<GR/camera.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>
#include<kinematics/body.hpp>
#include<diffgeomeng/classes/diff/Manifold.hpp>

using Tetrad = std::array<std::array<double, 4>, 4>;

class Observer{
public:
    Observer(GLFWwindow* window, Camera*, State<4>*, Manifold<4>*);

    void createTetrad();
    void update();
    static void keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods);
    void moveHandler(int key, int action);
    glm::vec4 getLocalSpeed();
    Body<4>* getBody();
    Camera* getCamera();
    Tetrad getTetrad();
    GRMetric<4>* getGRMetric();
    Manifold<4>* getManifold();
    void applyCameraRotation();

private:
    bool keys[255]{false};
    Camera* camera;
    Body<4>* body;
    Manifold<4>* manifold;
    Tetrad tetrad;
};