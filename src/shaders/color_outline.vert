#version 460 core

layout (location = 0) in vec2 vertex;

layout (std140) uniform Matrices
{
    mat4 modelViewProjection;
};

void main()
{
    gl_Position = modelViewProjection * vec4(vertex.x, vertex.y, 0.f, 1.f);
}