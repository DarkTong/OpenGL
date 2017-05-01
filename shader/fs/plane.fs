#version 330 core

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

out ver4 color;

void main()
{
    color = vec4(tecture(teture_diffuse1, TexCoords));
}