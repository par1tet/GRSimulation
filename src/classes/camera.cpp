#include <classes/camera.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

Camera::Camera(glm::vec3 position, GLFWwindow* window){
    this->position = position;

    this->direction = glm::vec3(0,0,1);
    this->upVector = glm::vec3(0,1,0);

    this->viewMatrix = glm::lookAt(this->position, this->position + this->direction, this->upVector);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, this->keyCallbackDispatcher);
}

void Camera::moveHandler(int key, int action)
{
    if (action == GLFW_PRESS) {
        this->keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        this->keys[key] = false;
    }  
}

void Camera::keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        
    if (cam) {
        cam->moveHandler(key, action);
    }
}

void Camera::update(){
    this->viewMatrix = glm::lookAt(this->position, this->position + this->direction, this->upVector);
    GLfloat cameraSpeed = 0.15f;
    
    if(this->keys['W']){
        this->position += cameraSpeed * this->direction;
    }
    if(this->keys['S']){
        this->position -= cameraSpeed * this->direction;
    }
    if(this->keys['A']){
        this->position -= glm::normalize(glm::cross(this->direction, this->upVector)) * cameraSpeed;
    }
    if(this->keys['D']){
        this->position += glm::normalize(glm::cross(this->direction, this->upVector)) * cameraSpeed;
    }
}