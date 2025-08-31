#version 150
 
out vec4 v_color;
 
void main()
{
    v_color = gl_Color;
    gl_Position = gl_Vertex;
}