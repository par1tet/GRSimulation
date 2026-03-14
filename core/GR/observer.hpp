#pragma once

#include<diffgeomeng/utility/types.hpp>
#include<GR/camera.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>
#include<kinematics/body.hpp>
#include<diffgeomeng/classes/diff/Manifold.hpp>

struct Tetrad{
    glm::mat4 e;
};

class Observer{
public:
    Observer(GLFWwindow* window, Camera*, State*, Manifold*);

    void createTetrad();
    void update();
    static void keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods);
    void moveHandler(int key, int action);
    glm::vec4 getLocalSpeed();
    Body* getBody();
    Camera* getCamera();
    Tetrad getTetrad();
    GRMetric* getGRMetric();
    Manifold* getManifold();

private:
    bool keys[255]{false};
    Camera* camera;
    Body* body;
    Manifold* manifold;
    Tetrad tetrad;
};