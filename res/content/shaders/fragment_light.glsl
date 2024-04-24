#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 TransformedNormal;
in vec3 FragPos;
in vec3 ViewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
};
struct Material {
    float shininess;
};

uniform Light light;
uniform Material material;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);

    //textures
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
    vec4 specularColor = texture(texture_specular1, TexCoords);

    float theta = dot(lightDir, normalize(-light.direction));

    if (theta > light.cutOff)
    {

        // ambient
        vec3 ambient = light.ambient * textureColor.rgb;

        // diffuse
        vec3 norm = normalize(TransformedNormal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * textureColor.rgb;

        // specular
        vec3 viewDir = normalize(-ViewPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * specularColor.rgb;

        // attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        //ambient  *= attenuation;
        diffuse   *= attenuation;
        specular *= attenuation;

        //final
        vec3 lightingEffect = ambient + diffuse + specular;
        FragColor = vec4(lightingEffect, textureColor.a);
        //FragColor = vec4(textureColor.rgb * lightingEffect, textureColor.a);
    }
    else
    {
        FragColor = vec4(light.ambient * textureColor.rgb, textureColor.a);
    }
}