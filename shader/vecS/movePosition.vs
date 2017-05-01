#version 330 core

//
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//输入位置
layout(location = 0) in vec3 local;
//输入纹理坐标
layout(location = 1) in vec2 coord;

// 输出颜色
//out vec4 ourColor;
// 输出纹理坐标
out vec2 texCoord;

void main()
{
    gl_Position = projection*view*model*vec4(local, 1.0f);
    //ourColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    texCoord = vec2(coord.x, 1.0f-coord.y);
}
