#include<string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <iostream>
#include <utils/shader_utils.h>
#include <mesh/sphereMesh.hpp>
#include <constans.h>
#include <kinematics/body.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <classes/camera.hpp>

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GRSim", nullptr, nullptr);

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

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int segments = 100;
    std::vector<float> circleMesh = generateSphereVertices(1, 36, 18);

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleMesh.size() * sizeof(float), circleMesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Camera* camera = new Camera({0, 0, 0}, window);

    std::vector<Body*> bodies = {
        new Body(glm::vec3{-5,0,-15},glm::vec3{0, 0, 0},glm::vec3{0}, 1, 1e13),
        new Body(glm::vec3{5,0,-16},glm::vec3{-1, 5, 0},glm::vec3{0}, 1, 1),
        new Body(glm::vec3{9,6,-10},glm::vec3{-1, 5, -1},glm::vec3{0}, 1, 1),
        new Body(glm::vec3{-5,2,8},glm::vec3{-1, -5, -1},glm::vec3{0}, 1, 1)
    };

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        system("clear");

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelM");

        glm::mat4 projection = glm::mat4(1);
        projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / HEIGHT, 0.1f, 1000.0f);
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projM");

        camera->update();
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewM");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // λδκφ ζν σδκ φελτκ γδ!
        for(int i = 0;i != bodies.size();i++){
            glm::mat4 model = glm::mat4(1);
            bodies[i]->update(dt, bodies, i);
            model = glm::translate(model, bodies[i]->position);

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLE_STRIP, 0, circleMesh.size() / 3);
        }
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    return 0;
}
