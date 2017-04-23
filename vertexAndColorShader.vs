#version 330 core

uniform vec3 shift;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 Coord;

out vec4 ourColor;
out vec4 myPosition;
out vec2 texCoord;

void main(){
    gl_Position = vec4(position.x+shift.x, position.y+shift.y, position.z+shift.z, 1.0);
    //myPosition = vec4(position.x+shift.x, position.y+shift.y, position.z+shift.z, 1.0);
    ourColor = vec4(color, 1.0f);
    texCoord = Coord;
}
