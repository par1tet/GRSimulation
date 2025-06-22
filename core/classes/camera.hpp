#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GLFW/glfw3.h>
#include<constans.h>

#pragma once

class Camera{
public:
    glm::vec3 position, direction, upVector;
    glm::mat4 viewMatrix;
    bool keys[255]{false};
    double lastX = WIDTH, lastY = HEIGHT;
    double yaw = 0.f, pitch = 0;
    double sens = 0.05;

    Camera(glm::vec3 position, GLFWwindow* window);
    void moveHandler(int key, int action);
    void dirHandler(double xpos, double ypos);
    static void keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void keyCallbackDispatcherMouse(GLFWwindow* window, double xpos, double ypos);
    void update();
};