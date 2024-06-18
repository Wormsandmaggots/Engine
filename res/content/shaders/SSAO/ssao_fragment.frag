#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
//rgb - diffuse, a - specular
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec3 gWorldPos;
//r - metallic, g - roughness, b - ambient
layout (location = 4) out vec3 gMetallicRoughnessAmbient;
layout (location = 5) out vec3 gEmissive;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_metalic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_emissive1;
uniform sampler2D texture_ambient1;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(texture_normal1, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    gPosition = FragPos;
    gNormal = getNormalFromMap();
    gAlbedo.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedo.a = texture(texture_specular1, TexCoords).r;
    gWorldPos = WorldPos;
    gEmissive = texture(texture_emissive1, TexCoords).rgb;
    gMetallicRoughnessAmbient.r = texture(texture_metalic1, TexCoords).r;
    gMetallicRoughnessAmbient.g = texture(texture_roughness1, TexCoords).g;
    gMetallicRoughnessAmbient.b = texture(texture_ambient1, TexCoords).b;
}