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
#include <GR/camera.hpp>
#include <GR/SpaceTime/Metrices/SchwarzschildMetric.hpp>
#include <GR/SpaceTime/spaceTime.hpp>
#include <kinematics/physicsEngine.hpp>
#include<GR/SpaceTime/Embeddings/SchwarzschildEmbedding.hpp>

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

    double mass = 1.f;

    SchwarzschildMetric* sZmetric = new SchwarzschildMetric(mass);
    SpaceTime* spaceTime = new SpaceTime(sZmetric, SchwarzchildEmbedding(mass));

    Camera* camera = new Camera({0, 0, 0}, window);

    std::vector<Body*> bodies = {
        new Body(new State({0, 20,0,M_PI/2},{1, 0, 0, 0}, 4),glm::vec4{0}, 1, 1),
    };

    PhysicsEngine* physEng = new PhysicsEngine(bodies, spaceTime);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::cout << "<-------------UPDATE-------------->" << std::endl;

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelM");

        glm::mat4 projection = glm::mat4(1);
        projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / HEIGHT, 0.1f, 1000.0f);
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projM");

        camera->update();
        physEng->update(dt);
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "viewM");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, circleMesh.size() / 3);

        // λδκφ ζν σδκ φελτκ γδ!
        for(int i = 0;i != bodies.size();i++){
            glm::mat4 model = glm::mat4(1);
            std::vector<double> x0 = bodies[i]->getState()->x0;

            std::vector<double> x0Emb = spaceTime->getManifold()->doEmbedding(x0);

            std::cout << "--EMBEDET COORDINATES--" << std::endl;
            std::cout << "t: " << x0Emb[0];
            std::cout << "x: " << x0Emb[1];
            std::cout << "y: " << x0Emb[2];
            std::cout << "z: " << x0Emb[3];
            std::cout << std::endl;

            glm::vec4 pos{x0Emb[0], x0Emb[1], x0Emb[2], x0Emb[3]};
            model = glm::translate(model, glm::vec3(pos.y, pos.z, pos.w));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLE_STRIP, 0, circleMesh.size() / 3);
        }
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    return 0;
}
