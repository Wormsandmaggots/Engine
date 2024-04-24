#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 TransformedNormal;
in vec3 FragPos;
in vec3 ViewPos;

uniform sampler2D texture_diffuse1;

//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct Material {
    //vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main()
{
    //texture
    vec4 textureColor = texture(texture_diffuse1, TexCoords);

    // ambient
    vec3 ambient = light.ambient * textureColor.rgb;

    // diffuse
    vec3 norm = normalize(TransformedNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * textureColor.rgb;

    // specular
    vec3 viewDir = normalize(-ViewPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
    vec3 specular = light.specular * (spec * material.specular);

    //final
    vec3 lightingEffect = ambient + diffuse + specular;
    FragColor = vec4(textureColor.rgb * lightingEffect, textureColor.a);
}