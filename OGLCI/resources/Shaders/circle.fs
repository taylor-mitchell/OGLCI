#version 460 core

layout(location = 0) out vec4 color;

in vec2 val;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;
void main() 
{
   float R = 1.0; 
   float dist = sqrt(dot(val,val));
   if (dist >= R)
   { discard; } 
   float alpha = smoothstep(R,R-0.01,dist);
   vec4 texColor = texture(u_Texture, v_TexCoord);
   color = texColor;
}