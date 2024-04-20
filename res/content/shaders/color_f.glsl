#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
uniform vec4 color;
uniform sampler2D nodes;

void main()
{
    vec4 nodesTexture = texture(nodes, TexCoords);
    FragColor = nodesTexture*color;
}
