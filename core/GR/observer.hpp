#pragma once

#include<diffgeomeng/utility/types.hpp>
#include<GR/camera.hpp>
#include<GR/SpaceTime/Metrices/GRMetric.hpp>
#include<kinematics/body.hpp>

struct Tetrad{
    glm::mat4 e;
};

class Observer{
public:
    Observer(GLFWwindow* window, Camera*, State*, Metric*);

    void createTetrad();
    void update();
    static void keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods);
    void moveHandler(int key, int action);
    glm::vec4 getLocalSpeed();
    Body* getBody();
    Camera* getCamera();
    Tetrad getTetrad();
private:
    bool keys[255]{false};
    Camera* camera;
    Body* body;
    Metric* metric;
    Tetrad tetrad;
};