#version 150
 
uniform vec2 resolution;
uniform vec2 size;
 
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
 
in vec4 v_color[];
 
out vec4 f_color;
out vec2 tex_coord;
 
void main()
{
    vec2 half_size = size / 2.f;
    half_size /= resolution;
    
    vec4 pos = gl_in[0].gl_Position;
    
    gl_Position = vec4(pos.x - half_size.x, pos.y - half_size.y, 0.f, 1.f);
    tex_coord = vec2(1.f, 1.f);
    f_color = v_color[0];
    EmitVertex();
 
    gl_Position = vec4(pos.x + half_size.x, pos.y - half_size.y, 0.f, 1.f);
    tex_coord = vec2(0.f, 1.f);
    f_color = v_color[0];
    EmitVertex();
 
    gl_Position = vec4(pos.x - half_size.x, pos.y + half_size.y, 0.f, 1.f);
    tex_coord = vec2(1.f, 0.f);
    f_color = v_color[0];
    EmitVertex();
 
    gl_Position = vec4(pos.x + half_size.x, pos.y + half_size.y, 0.f, 1.f);
    tex_coord = vec2(0.f, 0.f);
    f_color = v_color[0];
    EmitVertex();
    
    EndPrimitive();
}