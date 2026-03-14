#include<GR/observer.hpp>
#include<diffgeomeng/utility/functions.hpp>
#include<iostream>
#include<constans.h>

Observer::Observer(GLFWwindow* window, Camera* camera, State* state, Metric* metric){
    checkCorrectState(*state);

    this->metric = metric;
    this->camera = camera;
    this->body = new Body(state, glm::vec4{0}, 0, 0);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, this->keyCallbackDispatcher);
}

void Observer::moveHandler(int key, int action)
{
    if (action == GLFW_PRESS) {
        this->keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        this->keys[key] = false;
    }  
}

void Observer::keyCallbackDispatcher(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Observer* obs = static_cast<Observer*>(glfwGetWindowUserPointer(window));
        
    if (obs) {
        obs->moveHandler(key, action);
    }
}

void Observer::createTetrad(){
    State* state = this->body->getState();

    this->tetrad.e[0] = glm::vec4(state->v0[0], state->v0[1], state->v0[2], state->v0[3]);
    
}

void Observer::update(){
    this->createTetrad();

    camera->update();
    
    float obsSpeed = 1.5;
    glm::vec3 localSpeed{0};
    glm::vec3 camDir = this->camera->getDirection();
    glm::vec3 upVector = this->camera->getUpVector();
    Tetrad tetrad = this->tetrad;

    if(this->keys['W']){
        std::cout << "logW" << std::endl;
        localSpeed += obsSpeed * camDir;
    }
    if(this->keys['S']){
        localSpeed -= obsSpeed * camDir;
    }
    if(this->keys['A']){
        localSpeed -= glm::normalize(glm::cross(camDir, upVector)) * obsSpeed;
    }
    if(this->keys['D']){
        localSpeed += glm::normalize(glm::cross(camDir, upVector)) * obsSpeed;
    }

    State* state = this->body->getState();
    std::vector<double> newSpeed = state->v0;
    std::vector<double> newPos = state->x0;

    for(int i = 1;i != 4;i++){
        newSpeed[i] += localSpeed[i-1];
        newPos[i] += localSpeed[i-1] * dt;
    }

    this->body->setState(new State(newPos, newSpeed, 4));
}

Body* Observer::getBody(){
    return this->body;
}

Tetrad Observer::getTetrad(){
    return this->tetrad;
}

Camera* Observer::getCamera(){
    return this->camera;
}