#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

uniform mat4 u_MVP;

out vec3 v_color;
void main()
{
   gl_Position = u_MVP * position;
   v_color = color;
}