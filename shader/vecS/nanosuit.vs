#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

layout(std140) uniform Matrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
};

out VS_OUT
{
    vec2 TexCoords;
}vs_out;

//out vec2 TexCoords;
void main()
{
    vs_out.TexCoords = texCoords;
    gl_Position = projection * view * model * vec4(position, 1.0f);
    //gl_Position = vec4(position, 1.0f);
    //vs_out.Normal = normalize(transpose(inverse(mat3(view * model)))*normal);
    //vs_out.Position = position;
} 
