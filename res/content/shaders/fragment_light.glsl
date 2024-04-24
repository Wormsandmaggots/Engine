#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 TransformedNormal;
in vec3 FragPos;
in vec3 ViewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Material {
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 1 //nie może być zera

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 textureColor, vec4 specularColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 textureColor, vec4 specularColor);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 textureColor, vec4 specularColor);

void main()
{
    //properties
    vec3 norm = normalize(TransformedNormal);
    vec3 viewDir = normalize(-ViewPos);

    //textures
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
    vec4 specularColor = texture(texture_specular1, TexCoords);

    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir, textureColor, specularColor);

    // phase 2: point lights
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, textureColor, specularColor);

    // phase 3: spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, textureColor, specularColor);

    FragColor = vec4(result, 1.0);
}

//directional light
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 textureColor, vec4 specularColor)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * textureColor.rgb;
    vec3 diffuse = light.diffuse * diff * textureColor.rgb;
    vec3 specular = light.specular * spec * specularColor.rgb;
    return (ambient + diffuse + specular);
}

// point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 textureColor, vec4 specularColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * textureColor.rgb;
    vec3 diffuse = light.diffuse * diff * textureColor.rgb;
    vec3 specular = light.specular * spec * specularColor.rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// spot light
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 textureColor, vec4 specularColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * textureColor.rgb;
    vec3 diffuse = light.diffuse * diff * textureColor.rgb;
    vec3 specular = light.specular * spec * specularColor.rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity *50;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
