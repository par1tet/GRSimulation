#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 modelM;
uniform mat4 viewM;
uniform mat4 projM;

void main()
{
    gl_Position = projM * viewM * modelM * vec4(position.x, position.y, position.z, 1.0);
}