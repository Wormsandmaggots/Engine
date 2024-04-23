#version 460 core
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Material {
    float shininess;
};

struct Light {
    vec3 position;
    vec3 lightColor;
    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoords;
in vec3 TransformedNormal;
in vec3 FragPos;
in vec3 ViewPos;

uniform Material material;
uniform Light light;

void main()
{
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.lightColor;

    // diffuse
    vec3 norm = normalize(TransformedNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.lightColor;

    // specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.lightColor;

    //texture
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);
    vec4 specularColor = texture(texture_specular1, TexCoords);

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    vec3 lightingEffect = (ambient + diffuse + specular) * diffuseColor.rgb;
    FragColor = vec4(lightingEffect, diffuseColor.a);
}