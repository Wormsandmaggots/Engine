#version 330 core
out vec4 FragColor;

uniform vec3 color = vec3(0.0f, 1.0f, 0.0f); // domyślny kolor na zielony

void main()
{
    FragColor = vec4(color, 1.0f); // ustawienie koloru paska postępu
}