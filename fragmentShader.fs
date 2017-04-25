#version 330 core

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform float value;

//in vec4 ourColor;
in vec2 texCoord;

out vec4 color;

void main(){
    // texCoord(图片默认坐标是(0.0)->(1,1) ---- 然后纹理坐标是相对定点坐标所划出的与来进行分配的)
    color = mix(texture(ourTexture, vec2(texCoord.x, texCoord.y)),
        texture(ourTexture1, vec2(texCoord.x, texCoord.y)), 0.2);
}
