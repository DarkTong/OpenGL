#version 330 core

// 光源颜色
uniform vec3 lightColor;

out vec4 color;

void main()
{
    //color = vec4(lightColor, 1.0);
    color = vec4(1.0f);
}



/* error
#version 330 core

layout(location = 1) out vec4 color;

void main()
{
    color = vec4(1.0f);
}

*/
