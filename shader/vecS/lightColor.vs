#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 local;

void main()
{
    gl_Position = projection * view * model * vec4(local, 1.0f);
    //gl_Position = vec4(local, 1.0f);
}
