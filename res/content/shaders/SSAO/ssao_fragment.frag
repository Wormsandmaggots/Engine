#version 460 core

out vec4 FragColor;

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

uniform vec3 camPos;

struct LightColors {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight{
    vec3 direction;

    float intensity;
    LightColors colors;
};

struct SpotLight{
    vec3 position;
    LightColors colors;
    vec3 direction;
    float cutoff;
    float outercutoff;
};

struct PointLight {
    vec3 position;
    LightColors colors;

    float intensity;
    float radius;
    float linear;
    float quadratic;
};

uniform int material;
uniform vec4 materialColor;

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

DirectionalLight dirLight;

vec3 calculateDirLight2(DirectionalLight lightSource, vec3 normal, vec3 position, vec2 texCoord, float metallness,
                        float ao, float roughness, vec3 albedo, vec3 camPos);

const float PI = 3.14159265359;

#define MAX_LIGHTS 30

uniform PointLight[MAX_LIGHTS] pointLights;
uniform SpotLight[MAX_LIGHTS] spotLights;

uniform int spotNum = 0;
uniform int pointNum = 0;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float calculateSpotLight(vec3 spotLightPos, vec3 spotLightDir, float cutOff, float outerCutOff, vec3 color, vec3 normal, vec3 fragPos, vec3 viewDir);



void main()
{
    gPosition = FragPos;
    gNormal = getNormalFromMap();
    //gNormal = texture(texture_normal1,TexCoords).rgb;
    //gNormal = normalize(Normal);
    //gAlbedo.rgb = texture(texture_diffuse1, TexCoords).rgb;
    
    // store specular intensity in gAlbedoSpec's alpha component
    //gAlbedo.a = texture(texture_specular1, TexCoords).r;
    gWorldPos = WorldPos;
    gEmissive = texture(texture_emissive1, TexCoords).rgb;
    gMetallicRoughnessAmbient.r = texture(texture_metalic1, TexCoords).r;
    gMetallicRoughnessAmbient.g = texture(texture_roughness1, TexCoords).g;
    gMetallicRoughnessAmbient.b = texture(texture_ambient1, TexCoords).b;

    vec3 albedo     = pow(texture(texture_diffuse1, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(texture_metalic1, TexCoords).r;
    float roughness = texture(texture_roughness1, TexCoords).r;
    float ao        = texture(texture_ambient1, TexCoords).r;

    vec3 N = getNormalFromMap();
    //vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // init reflectance equation
    vec3 Lo = vec3(0.0);

    vec3 L = vec3(0);
    vec3 H = vec3(0);
    float distance = 0;
    float attenuation = 0;
    vec3 radiance = vec3(0);

    // Cook-Torrance BRDF


    float NDF = 0;
    float G   = 0;
    vec3 F    = vec3(0);

    vec3 numerator    = vec3(0);
    float denominator =  0;
    vec3 specular = vec3(0);

    // kS is equal to Fresnel
    vec3 kS = vec3(0);
    // energy conservation
    vec3 kD = vec3(0);

    float NdotL = 0;

    for(int i = 0; i < pointNum; i++)
    {
         L = normalize(pointLights[i].position - WorldPos);
         H = normalize(V + L);
         distance = length(pointLights[i].position - WorldPos);
         attenuation = 1.0 / (distance * distance);
         radiance = pointLights[i].colors.diffuse * attenuation;

        // Cook-Torrance BRDF


         NDF = DistributionGGX(N, H, roughness);
         G   = GeometrySmith(N, V, L, roughness);
         F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

         numerator    = NDF * G * F;
         denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
         specular = numerator / denominator;

        // kS is equal to Fresnel
        kS = F;
        // energy conservation
        kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }


    // calculate directional light radiance
    L = normalize(-dirLight.direction);
    H = normalize(V + L);
    radiance = dirLight.colors.diffuse;

    // Cook-Torrance BRDF
    NDF = DistributionGGX(N, H, roughness);
    G   = GeometrySmith(N, V, L, roughness);
    F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    numerator    = NDF * G * F;
    denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    specular = numerator / denominator;

    // kS is equal to Fresnel
    kS = F;
    // energy conservation
    kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    //spotLight
    for(int i = 0; i < spotNum; i++)
    {
        SpotLight s = spotLights[i];
        float spotlightIntensity = calculateSpotLight(s.position, s.direction, s.cutoff, s.outercutoff, s.colors.diffuse, N, WorldPos, V);
        color += spotlightIntensity;
    }

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    //vec3 finalColor = cel(color);
    if(material == 1){
    FragColor = vec4(color, 1.0)*materialColor;
    gAlbedo = vec4(color, 1.0)*materialColor;
    }else{
    FragColor = vec4(color, 1.0);
    gAlbedo = vec4(color, 1.0);
    }

}

vec3 calculateDirLight2(DirectionalLight lightSource, vec3 normal, vec3 position, vec2 texCoord, float metallness,
                        float ao, float roughness, vec3 albedo, vec3 camPos)
{

    vec3 lightDir = normalize(-lightSource.direction);
    vec3 radiance = lightSource.colors.diffuse * lightSource.intensity;

    vec3 cameraDir = normalize(camPos - position);
    vec3 halfway = normalize(lightDir + cameraDir);
    float cosTheta = max(dot(halfway, cameraDir), 0.0);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallness);

    vec3 F = fresnelSchlick(cosTheta, F0);
    float D = DistributionGGX(normal, halfway, roughness);
    float G = GeometrySmith(normal, cameraDir, lightDir, roughness);

    float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotV = max(dot(normal, cameraDir), 0.0);

    vec3 numerator = D * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallness);

    //float shadow = calculateShadow(position, normal);

    vec3 result = (kD * albedo / PI + specular) * radiance * NdotL * ao;
    result = result + lightSource.colors.ambient * albedo * ao;

    //result = result * (1.0 - shadow) + light.colors.ambient * albedo * ao;

    //    return normal;
    return result;
}

float calculateSpotLight(vec3 spotLightPos, vec3 spotLightDir, float cutOff, float outerCutOff, vec3 color, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightToFragment = normalize(spotLightPos - fragPos);
    float theta = dot(lightToFragment, -spotLightDir);
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

    // rest of the lighting calculations (ambient, diffuse, specular)
    vec3 lightColor = color; // adjust this as needed
    float ambientStrength = 0.1; // adjust this as needed
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(spotLightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5; // adjust this as needed
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * intensity;
    float finalIntensity = max(result.r, max(result.g, result.b));

    return finalIntensity;
}