#version 460 core

out vec4 FragColor;

uniform vec4 outlineColor = vec4(1.f, 1.f, 1.f, 1.f);

void main()
{
    FragColor = outlineColor;
}