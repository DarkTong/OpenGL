#version 330 core

uniform sampler2D texture_diffuse1;

//in ver3 Normal;
in vec2 TexCoords;
in vec3 Color;

out vec4 color;

float LinearizeDepth(float depth);

void main()
{
    color = vec4(texture(texture_diffuse1, TexCoords));
    //color = vec4(vec3(gl_FragCoord.z), 1.0f);
    //color = vec4(Color, 1.0f);
    //float depth = LinearizeDepth(gl_FragCoord.z) / far;
    //color = vec4(vec3(depth), 1.0);
}

float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far = 50.0;
    float z = depth * 2.0 - 1.0;
    return (2.0 * near) / (far + near - z * (far - near));
}
