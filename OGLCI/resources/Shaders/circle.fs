#version 460 core

layout(location = 0) out vec4 color;

in vec2 val;
in vec3 u_color;
void main() 
{
   float R = 1.0; 
   float dist = sqrt(dot(val,val));
   if (dist >= R)
   { discard; } 
   float alpha = smoothstep(R,R-0.01,dist);
   color = vec4(u_color, alpha);
}