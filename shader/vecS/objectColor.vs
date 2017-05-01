#version 330 core

// 空间参数
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// 观测者位置
uniform vec3 viewPos;

// 法向量
out vec3 Normal;
// 物体世界空间位置
out vec3 FragPos;
// 摄像机世界空间位置
out vec3 ViewPos;
// 纹理坐标
out vec2 TexCoords;

// 定点信息
layout(location = 0) in vec3 local;
// 法向量
layout(location = 1) in vec3 normal;
// 纹理坐标
layout(location = 2) in vec2 texCoords;

void main()
{
    gl_Position = projection * view * model * vec4(local, 1.0f);
    Normal = normalize( transpose(inverse(mat3(model)))* normal);
    FragPos = vec3(model * vec4(local, 1.0f));
    ViewPos = viewPos;
    TexCoords = texCoords;
}
