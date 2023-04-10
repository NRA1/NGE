#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 widget;

void main()
{
    gl_Position = projection * widget * vec4(aPos, 1.0);
}