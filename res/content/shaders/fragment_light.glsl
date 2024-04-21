#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 TransformedNormal;
in vec3 FragPos;
in vec3 ViewPos;

uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(TransformedNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5; //0.5
    vec3 viewDir = normalize(-ViewPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //increase 32 to be more specular
    vec3 specular = specularStrength * spec * lightColor;

    //texture
    vec4 textureColor = texture(texture_diffuse1, TexCoords);

    vec3 lightingEffect = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(textureColor.rgb * lightingEffect, textureColor.a);
}