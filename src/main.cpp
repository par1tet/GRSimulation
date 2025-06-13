#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <iostream>

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLSim", nullptr, nullptr);

    if (window == nullptr){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        return -1;
    }

    int width, heigth;
    glfwGetFramebufferSize(window, &width, &heigth);

    glViewport(0,0, width, heigth);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}
