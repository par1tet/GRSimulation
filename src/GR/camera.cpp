#include <GR/camera.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GR/observer.hpp>

Camera::Camera(GLFWwindow* window){
    this->lastX = WIDTH / 2;
    this->lastY = HEIGHT / 2;

    this->yaw = -90.f;
    this->pitch = 0.f;
    this->sens = 0.05;

    glm::vec3 front;

    front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    front.y = sin(glm::radians(this->pitch));
    front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));

    this->direction = glm::normalize(front);
    this->upVector = glm::vec3(0,1,0);

    glfwSetCursorPosCallback(window, this->keyCallbackDispatcherMouse);
}

void Camera::update(){
    glm::vec3 front;
    front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    front.y = sin(glm::radians(this->pitch));
    front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    this->direction = glm::normalize(front);
}

void Camera::keyCallbackDispatcherMouse(GLFWwindow* window, double xpos, double ypos){
    Observer* obs = static_cast<Observer*>(glfwGetWindowUserPointer(window));
        
    if (obs) {
        obs->getCamera()->dirHandler(xpos, ypos);
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
    std::cout << "movemouse" << std::endl;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}

glm::vec3 Camera::getDirection(){
    return this->direction;
}

glm::vec3 Camera::getUpVector(){
    return this->upVector;
}