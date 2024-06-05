#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    float r = texture(screenTexture, TexCoords).r;
    float c = step(0., sin(TexCoords.x * 10. + r * 40.));

    FragColor = vec4(vec3(c), 1.0);
}