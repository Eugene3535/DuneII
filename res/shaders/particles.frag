#version 150
 
uniform sampler2D texture;
 
in vec2 tex_coord;
in vec4 f_color;
 
out vec4 total_color;
 
void main()
{
    total_color = texture2D(texture, tex_coord) * f_color;
}