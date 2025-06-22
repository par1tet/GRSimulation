#include <classes/camera.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

Camera::Camera(glm::vec3 position, GLFWwindow* window){
    this->position = position;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->direction = glm::normalize(front);
    this->upVector = glm::vec3(0,1,0);

    this->viewMatrix = glm::lookAt(this->position, this->position + this->direction, this->upVector);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, this->keyCallbackDispatcher);
    glfwSetCursorPosCallback(window, this->keyCallbackDispatcherMouse);
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
    GLfloat cameraSpeed = 0.4f;
    
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

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->direction = glm::normalize(front);
}

void Camera::keyCallbackDispatcherMouse(GLFWwindow* window, double xpos, double ypos){
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        
    if (cam) {
        cam->dirHandler(xpos, ypos);
    }
}

void Camera::dirHandler(double xpos, double ypos){
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    this->lastX = xpos;
    this->lastY = ypos;

    xoffset *= this->sens;
    yoffset *= this->sens;     

    this->yaw += xoffset;
    this->pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}