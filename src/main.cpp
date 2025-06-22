#include<string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <iostream>
#include <utils/shader_utils.h>
#include <mesh/circleMesh.h>
#include <constans.h>

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLSim", nullptr, nullptr);

    if (window == nullptr){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        return -1;
    }

    GLuint shaderProgram = createShaderProgram(
      	std::string(std::string(CMAKE_SOURCE_PATH) + "src/shaders/vertex_shader.glsl").c_str(),
      	std::string(std::string(CMAKE_SOURCE_PATH) + "src/shaders/fragment_shader.glsl").c_str()
    );    

    int width, heigth;
    glfwGetFramebufferSize(window, &width, &heigth);

    glViewport(0,0, width, heigth);

    int segments = 100;
    std::vector<float> circleMesh = getCircleMesh(100, segments);


    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleMesh.size() * sizeof(float), circleMesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VAO);

    glBindVertexArray(0);



    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleMesh.size() / 3);

        glfwSwapBuffers(window);
    }

    return 0;
}
