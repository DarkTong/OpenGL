#version 330 core
/*
 * 经典定点着色器配置
 */
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

out VS_OUT
{
    out vec3 FragPos;   // 世界空间中的
    out vec3 Normal;
    out vec2 TexCoords;
    out vec4 FragPosLightSpace;
}vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    //gl_Position = vec4(position, 1.0);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = normalize(transpose(inverse(mat3(model))) * normal);
    vs_out.TexCoords = texCoords;
    // 换句话说，这是光源空间的向量
    vs_out.FragPosLightSpace = lightSpaceMatrix * model * vec4(position, 1.0);
}
