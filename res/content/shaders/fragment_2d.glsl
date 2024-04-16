#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    vec2 wrappedTexCoord = fract(TexCoord); // zawijanie koordynat tekstury
    FragColor = texture(texture1, wrappedTexCoord);
}