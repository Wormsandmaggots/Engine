#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_metalness1;

struct material{
    sampler2D texture_specular;
    sampler2D texture_diffuse;
    sampler2D texture_metalness;
};

uniform material[] materials;

void main()
{
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);
    float metallic  = texture(metallicMap, TexCoords).r;
    vec3 finalColor = mix(diffuseColor.rgb, metalic);

    //FragColor = vec4(finalColor, diffuseColor.a);
    FragColor = vec4(finalColor, TexCoords);
    //FragColor = texture(texture_diffuse1, TexCoords);
}