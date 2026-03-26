#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, TexCoord).rgb;

    // tone mapping (очень важно для рейтрейсинга)
    color = color / (color + vec3(1.0));

    // gamma correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}