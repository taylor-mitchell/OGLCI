#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 value;
layout(location = 2) in vec3 color;

uniform mat4 u_MVP;

out vec2 val;
out vec3 u_color;
void main()
{
   val = value;
   u_color = color;
   gl_Position = u_MVP * position;
}