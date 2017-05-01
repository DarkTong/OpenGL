#version 330 core
// 材质结构
struct Material
{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
// 光照的基本参数
struct ColorBase
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// 平行光结构
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// 光源结构
struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};
// 手电筒
struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
};

#define NR_POINT_LIGHTS 4

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

out vec4 color;

// 物体位置
in vec3 FragPos;
// 法向量（单位向量）
in vec3 Normal;
// 观测者位置
in vec3 ViewPos;
// 纹理坐标
in vec2 TexCoords;

// 初始化值
ColorBase initCB;

// func
ColorBase CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
ColorBase CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
ColorBase CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
    initCB.ambient = initCB.diffuse = initCB.specular = vec3(0.0);

    // 最终结果的暂存变量
    ColorBase tColor, tmp;
    // 观测方向(摄像机->物体)
    vec3 viewDir = normalize(FragPos - ViewPos);
    // 法向量(标准化（安全）)
    vec3 normal = normalize(Normal);

    // 平行光对光照的贡献
    tmp = CalcDirLight(dirLight, normal, viewDir);
    tColor.ambient += tmp.ambient;
    tColor.diffuse += tmp.diffuse;
    tColor.specular += tmp.specular;
    // 定点光对光照的贡献
    for(int i=0;i<NR_POINT_LIGHTS;++i)
    {
        tmp = CalcPointLight(pointLight[i], normal, viewDir, FragPos);
        tColor.ambient += tmp.ambient;
        tColor.diffuse += tmp.diffuse;
        tColor.specular += tmp.specular;
    }
    // 手电光对光照的贡献
    tmp = CalcSpotLight(spotLight, normal, viewDir);
    tColor.ambient += tmp.ambient;
    tColor.diffuse += tmp.diffuse;
    tColor.specular += tmp.specular;
    // 添加材质
    tColor.ambient = tColor.ambient * vec3(texture(material.diffuse, TexCoords));
    tColor.diffuse = tColor.diffuse * vec3(texture(material.diffuse, TexCoords));
    tColor.specular = tColor.specular * vec3(texture(material.specular, TexCoords));
    vec3 result = tColor.ambient + tColor.diffuse + tColor.specular;
    color = vec4(result, 1.0f);
}
// 平行光对光照的贡献
ColorBase CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    ColorBase color = initCB;
    // 环境光
    color.ambient = light.ambient;
    // 漫反射光
    color.diffuse = light.diffuse * max(dot(normal, -light.direction), 0.0);
    // 镜面高光
    vec3 reflectDir = reflect(- light.direction, normal);
    color.specular = light.specular * pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess);
    // 返回平行光贡献
    return color;
}
// 定点对光照的贡献
ColorBase CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    ColorBase color = initCB;
    // 计算光线衰减程度
    // -距离
    float distance = length(fragPos - light.position);
    // -光强
    float a = light.constant;
    float b = light.linear;
    float c = light.quadratic;
    float attenuation = 1.0/(a + b*distance + c*distance*distance);
    // 环境光
    color.ambient = light.ambient * attenuation;
    // 漫反射光(源->目标)
    vec3 lightDir = normalize(fragPos - light.position);
    color.diffuse = max(dot(normal, -lightDir), 0.0) * light.diffuse * attenuation;
    // 镜面高光
    vec3 reflectDir = reflect(lightDir, normal);
    color.specular = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess) * light.specular * attenuation;

    return color;
}
// 手电光对光照的贡献
ColorBase CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    ColorBase color = initCB;
    // 计算光线是否在可是范围内
    // -获取角度
    float theta = dot(light.direction, viewDir);
    // -利用公式计算（theta - outCutOff)/(cutOff - outerCutOff)
    float epsilon = light.cutOff - light.outerCutOff;
    // 光照强度
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);
    // 环境光
    color.ambient = light.ambient * intensity;
    // 漫反射光
    color.diffuse = light.diffuse * max(dot(normal, -light.direction), 0.0) * intensity;
    // 镜面高光
    vec3 reflectDir = reflect(viewDir, normal);
    color.specular = light.specular * pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess) * intensity;
    
    return color;
}











