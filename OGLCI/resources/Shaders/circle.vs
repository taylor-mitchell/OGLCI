#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 value;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_MVP;

out vec2 val;
out vec2 v_TexCoord;
void main()
{
   val = value;
   v_TexCoord = texCoord;
   gl_Position = u_MVP * position;
}