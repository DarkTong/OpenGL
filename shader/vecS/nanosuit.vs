#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoords;
out vec3 Position;

void main()
{
    //gl_Position = projection * view * model * vec4(position, 1.0f);
    gl_Position = vec4(position, 1.0);
    Normal = normalize(transpose(inverse(mat3(view * model)))*normal);
    TexCoords = texCoords;
    Position = position;
} 
