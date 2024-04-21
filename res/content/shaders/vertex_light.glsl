#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 TransformedNormal;
out vec3 FragPos;
out vec3 ViewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    TexCoords = aTexCoords;
    TransformedNormal = normalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    ViewPos = vec3(view * model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}