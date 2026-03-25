#include<GR/observer.hpp>
#include<diffgeomeng/utility/functions.hpp>
#include<iostream>
#include<constans.h>

Observer::Observer(GLFWwindow* window, Camera* camera, State<4>* state, Manifold<4>* manifold){
    checkCorrectState(*state);

    this->manifold = manifold;
    this->camera = camera;
    this->body = new Body<4>(state, glm::vec4{0}, 0, 0);

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

double dot(const std::array<std::array<double,4>,4>& g, std::array<double,4> a, std::array<double,4> b){
    double sum = 0;

    for(int i = 0;i != 4;i++){
        for(int j = 0;j != 4;j++){
            sum+=g[i][j] * a[i] * b[j];
        }
    }
    
    return sum;
}

std::array<double, 4> normalize(const std::array<std::array<double,4>,4>& g, std::array<double,4> v){
    std::array<double, 4> n{};

    double norm = 1 / sqrt(abs(dot(g, v, v)));

    for(int i = 0;i != 4;i++){
        n[i] = v[i] * norm;
    }

    return n;
}

void Observer::createTetrad(){
    State<4>* state = this->body->getState();

    std::array<std::array<double, 4>, 4> g = this->getManifold()->getMetric()->getMatrixAtPoint(state->x0);

    this->tetrad[0] = state->v0;

    this->tetrad[1] = {0, 1, 0, 0};
    this->tetrad[2] = {0, 0, 1, 0};
    this->tetrad[3] = {0, 0, 0, 1};

    for(int i = 1;i != 4;i++){
        for(int j = 0;j != 4;j++){
            this->tetrad[i][j] -= (dot(g, state->v0, this->tetrad[i])) * state->v0[j];
        }

        for(int k = 1;k < i;k++){
            for(int j = 0;j != 4;j++){
                this->tetrad[i][j] += (dot(g, this->tetrad[k], this->tetrad[i])) * this->tetrad[k][j];
            }
        }

        this->tetrad[i] = normalize(g, this->tetrad[i]);
    }
}

void Observer::update(){
    this->createTetrad();

    camera->update();
    
    float obsSpeed = 1.5;
    glm::vec3 localSpeed{0};
    glm::vec3 camDir = this->camera->getDirection();
    glm::vec3 upVector = this->camera->getUpVector();

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

    State<4>* state = this->body->getState();
    Point<4> newSpeed = state->v0;
    Point<4> newPos = state->x0;

    for(int i = 1;i != 4;i++){
        newSpeed.x[i] += localSpeed[i-1];
        newPos.x[i] += localSpeed[i-1] * dt;
    }

    this->body->setState(new State<4>(newPos.x, newSpeed.x));
}

Body<4>* Observer::getBody(){
    return this->body;
}

Tetrad Observer::getTetrad(){
    return this->tetrad;
}

Camera* Observer::getCamera(){
    return this->camera;
}

GRMetric<4>* Observer::getGRMetric(){
    GRMetric<4>* grMetric = dynamic_cast<GRMetric<4>*>(this->manifold->getMetric());

    if(grMetric){
        return grMetric;
    }else{
        std::cerr << "Metric for manifold must be GRMetric";
    }

    return nullptr;
}

Manifold<4>* Observer::getManifold(){
    return this->manifold;
}

