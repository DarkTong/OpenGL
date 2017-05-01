#version 330 core
/*
 * 经典定点着色器配置
 */
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

uniform vec3 model;
uniform vec3 view;
uniform vec3 projection;

out TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;
}