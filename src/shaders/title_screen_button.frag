#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform vec4 buttonColor = vec4(1.f, 1.f, 1.f, 1.f);
uniform sampler2D texture0;

void main()
{
    vec4 texColor = texture(texture0, texCoords) * buttonColor;

    if(texColor.a < 0.1f)
        discard;

    FragColor = texColor;
}