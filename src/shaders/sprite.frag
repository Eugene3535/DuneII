#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D texture0;

void main()
{
    vec4 texColor = texture(texture0, texCoords);

    if(texColor.a < 0.99f)
        discard;

    FragColor = texColor;
}