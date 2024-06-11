#version 330 core


#define MAX_LIGHTS 30

out vec4 FragColor;


in vec2 TexCoords;


struct PointLight {
    vec3 position;
    vec3 color;

    float radius;
    float linear;
    float quadratic;
};

struct DirectionalLight{
    vec3 direction;
    vec3 color;
};

struct SpotLight{
    vec3 position;
    vec3 color;
    vec3 direction;
    float cutoff;
    float outercutoff;
};


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;
uniform sampler2D gWorldPos;
uniform sampler2D gMetallicRoughnessAmbient;
uniform sampler2D gEmissive;


uniform vec3 camPos;


uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform DirectionalLight dirLight;


uniform PointLight[MAX_LIGHTS] pointLights;
uniform SpotLight[MAX_LIGHTS] spotLights;


uniform int spotNum;
uniform int pointNum;

const float PI = 3.14159265359;


float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
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

float calculateSpotLight(vec3 spotLightPos, vec3 spotLightDir, float cutOff, float outerCutOff, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightToFragment = normalize(spotLightPos - fragPos);
    float theta = dot(lightToFragment, -spotLightDir);
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

    // rest of the lighting calculations (ambient, diffuse, specular)
    vec3 lightColor = vec3(1.0, 1.0, 1.0); // adjust this as needed
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

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2f));
    float Specular = texture(gAlbedo, TexCoords).a;
    vec3 WorldPos = texture(gWorldPos, TexCoords).rgb;
    vec3 Emissive = texture(gEmissive, TexCoords).rgb;
    float Metallic = texture(gMetallicRoughnessAmbient, TexCoords).r;
    float Roughness = texture(gMetallicRoughnessAmbient, TexCoords).g;
    float AmbientOcclusion = texture(gMetallicRoughnessAmbient, TexCoords).b;

    vec3 V = normalize(camPos - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);

    // init reflectance equation
    vec3 Lo = vec3(0.0);

    // calculate light radiance
    vec3 L = normalize(pointLight.position - WorldPos);
    vec3 H = normalize(V + L);
    float distance = length(pointLight.position - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = pointLight.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Roughness);
    float G   = GeometrySmith(Normal, V, L, Roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // energy conservation
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - Metallic;

    // scale light by NdotL
    float NdotL = max(dot(Normal, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * Albedo / PI + specular) * radiance * NdotL;

    // calculate directional light radiance
    L = normalize(-dirLight.direction);
    H = normalize(V + L);
    radiance = dirLight.color;

    // Cook-Torrance BRDF
    NDF = DistributionGGX(Normal, H, Roughness);
    G   = GeometrySmith(Normal, V, L, Roughness);
    F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    numerator    = NDF * G * F;
    denominator = 4.0 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    specular = numerator / denominator;

    // kS is equal to Fresnel
    kS = F;
    // energy conservation
    kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - Metallic;

    // scale light by NdotL
    NdotL = max(dot(Normal, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * Albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * Albedo * AmbientOcclusion;

    vec3 color = ambient + Lo;

    float spotlightIntensity = calculateSpotLight(spotLight.position, spotLight.direction, spotLight.cutoff, spotLight.outercutoff, Normal, WorldPos, V);
    color += spotlightIntensity;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    //vec3 finalColor = cel(color);
    FragColor = vec4(color, 1.0);
}