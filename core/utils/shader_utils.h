#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <string>
#include <glad/glad.h>

std::string loadShaderSource(const char* filePath);
GLuint createShader(const char* filePath, GLenum shaderType);
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);
GLuint loadTexture(const char* filePath);

#endif