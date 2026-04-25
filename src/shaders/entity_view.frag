#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D texture0;
uniform float top;
uniform float bottom;
uniform float progress;

void main()
{
    float localY = 1.f - (texCoords.y - top) / (bottom - top);
    float fillHeight = progress * 0.01f;
    vec4 pixelColor = texture(texture0, texCoords);
    
    if (localY < fillHeight) 
    {
        FragColor = mix(pixelColor, vec4(1.0, 1.0, 1.0, 1.0), 0.6f);
    } 
    else 
    {
        FragColor = pixelColor;
    }
}