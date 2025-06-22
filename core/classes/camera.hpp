#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GLFW/glfw3.h>

#pragma once

class Camera{
public:
    glm::vec3 position, direction, upVector;
    glm::mat4 viewMatrix;
    bool keys[255]{false};

    Camera(glm::vec3 position, GLFWwindow* window);
    void moveHandler(int key, int action);
    static void keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods);
    void update();
};