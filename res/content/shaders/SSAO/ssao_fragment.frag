#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gWorldPos;
layout (location = 4) out vec3 gMetallicRoughnessAmbient;
layout (location = 5) out vec4 gEmissive;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_metallic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_emissive1;
uniform sampler2D texture_ambient1;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedo.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedo.a = texture(texture_specular1, TexCoords).r;
    gWorldPos = WorldPos;
    gMetallicRoughnessAmbient.r = texture(texture_metallic1, TexCoords).r;
    gEmissive = texture(texture_emissive1, TexCoords);
    gMetallicRoughnessAmbient.g = texture(texture_roughness1, TexCoords).g;
    gMetallicRoughnessAmbient.b = texture(texture_ambient1, TexCoords).b;
}