#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec4 color;

void main()
{
    vec4 texColor = texture(texture1, TexCoords);
    FragColor = color * texColor;
}