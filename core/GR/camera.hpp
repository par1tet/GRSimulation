#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GLFW/glfw3.h>
#include<constans.h>

#pragma once

class Camera{
public:
    Camera(GLFWwindow* window);
    void moveHandler(int key, int action);
    void dirHandler(double xpos, double ypos);
    static void keyCallbackDispatcherMouse(GLFWwindow* window, double xpos, double ypos);
    void update();
    glm::vec3 getUpVector();
    glm::vec3 getDirection();
    double getYaw();
    double getPitch();

private:
    glm::vec3 direction, upVector;
    double lastX, lastY;
    double yaw, pitch;
    double sens;
};