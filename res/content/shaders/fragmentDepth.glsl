#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D depthMap;

void main()
{
    // Pobieramy wartość głębokości
    float depthValue = texture(depthMap, TexCoords).r;

    // Używamy wartości głębokości jako wartości szarości
    FragColor = vec4(vec3(depthValue), 1.0);
}