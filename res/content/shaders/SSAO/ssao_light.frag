#version 330 core


#define MAX_LIGHTS 30

out vec4 FragColor;


in vec2 TexCoords;


struct LightColors {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct PointLight {
    vec3 position;
    LightColors colors;

    float intensity;
    float radius;
    float linear;
    float quadratic;
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


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;
uniform sampler2D gWorldPos;
uniform sampler2D gMetallicRoughnessAmbient;
uniform sampler2D gEmissive;


uniform vec3 camPos;
//perspective = projection * view
uniform mat4 view;
uniform mat4 projection;


uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform DirectionalLight dirLight;


uniform PointLight[MAX_LIGHTS] pointLights;
uniform SpotLight[MAX_LIGHTS] spotLights;


uniform int spotNum = 0;
uniform int pointNum = 0;


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

float D_GGX(in float roughness, in float NdH) {
    float m = roughness * roughness;
    float m2 = m * m;
    float d = (NdH * m2 - NdH) * NdH + 1.0;
    return m2 / (PI * d * d);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float G_schlick(in float roughness, in float NdV, in float NdL) {
    float k = roughness * roughness * 0.5;
    float V = NdV * (1.0 - k) + k;
    float L = NdL * (1.0 - k) + k;
    return 0.25 / (V * L);
}


vec3 cooktorrance_specular(in float NdL, in float NdV, in float NdH, in vec3 specular, in float roughness) {
    float D = D_GGX(roughness, NdH);

    float G = G_schlick(roughness, NdV, NdL);

    float rim = mix(1.0 - roughness * texture(gMetallicRoughnessAmbient, TexCoords).w * 0.9, 1.0, NdV);

    return (1.0 / rim) * specular * G * D;
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

vec3 FresnelSchlick(float cosTheta, vec3 albedo, float metallness) {
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallness);

    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 calculateDirLight(DirectionalLight lightSource, vec3 normal, vec3 position, vec2 texCoord)
{
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    vec3 metalnessAORoughness = texture(gMetallicRoughnessAmbient, texCoord).rgb;

    //vec3 albedo = albedoMetal.rgb;
    float metallness = metalnessAORoughness.r;
    float ao = metalnessAORoughness.g;
    float roughness = metalnessAORoughness.b;

    vec3 lightDir = normalize(-lightSource.direction);
    vec3 radiance = lightSource.colors.diffuse * lightSource.intensity;

    vec3 cameraDir = normalize(camPos - position);
    vec3 halfway = normalize(lightDir + cameraDir);
    float cosTheta = max(dot(halfway, cameraDir), 0.0);

    vec3 F = FresnelSchlick(cosTheta, albedo, metallness);
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

float calculateSpotLight(SpotLight lightSource, vec3 normal, vec3 position)
{
    vec3 cameraDir = normalize((camPos - position));
    vec3 lightToFragment = normalize(lightSource.position - position);
    float theta = dot(lightToFragment, -lightSource.direction);
    float epsilon = lightSource.cutoff - lightSource.outercutoff;
    float intensity = clamp((theta - lightSource.outercutoff) / epsilon, 0.0, 1.0);

    // rest of the lighting calculations (ambient, diffuse, specular)
    vec3 lightColor = lightSource.colors.diffuse; // adjust this as needed
    float ambientStrength = 0.1; // adjust this as needed
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightSource.position - position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5; // adjust this as needed
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * intensity;
    float finalIntensity = max(result.r, max(result.g, result.b));

    return finalIntensity;
}

vec3 calculatePointLight(PointLight lightSource, vec3 normal, vec3 position, vec2 texCoord)
{
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    vec3 metalnessAORoughness = texture(gMetallicRoughnessAmbient, texCoord).rgb;

    //vec3 albedo = albedoMetal.rgb;
    float metallness = metalnessAORoughness.r;
    float ao = metalnessAORoughness.g;
    float roughness = metalnessAORoughness.b;

    vec3 lightVec = lightSource.position - position;
    vec3 lightDir = normalize(lightVec);

    float distance = length(lightVec);

    if(distance > lightSource.radius)
            return vec3(0);

    float attenuation = 1.0 /  (distance * distance);
    vec3 radiance = lightSource.colors.diffuse * attenuation * lightSource.intensity;

    //vec3 cameraDir = normalize(camPos - position);
    vec3 cameraDir = normalize((camPos - position));
    vec3 halfway = normalize(lightDir + cameraDir);
    float cosTheta = max(dot(halfway, cameraDir), 0.0);

    vec3 F = FresnelSchlick(cosTheta, albedo, metallness);
    float D = DistributionGGX(normal, halfway, roughness);
    float G = GeometrySmith(normal, cameraDir, lightDir, roughness);

    float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotV = max(dot(normal, cameraDir), 0.0);

    vec3 numerator = D * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallness);

    vec3 result = (kD * albedo / PI + specular) * radiance * NdotL * ao;

    return result;
}

vec3 calculatePosition(vec2 texCoord) {
    float depth = texture(gWorldPos, texCoord).z;

    vec4 clipSpacePosition = vec4(vec3(texCoord, depth) * 2.0 - 1.0, 1.0);
    vec4 viewSpacePosition = inverse(projection * view) * clipSpacePosition;

    return viewSpacePosition.xyz / viewSpacePosition.w;
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
        float AmbientOcclusion = (texture(gMetallicRoughnessAmbient, TexCoords).b + texture(ssao, TexCoords).r);

        vec2 texCoord = gl_FragCoord.xy / vec2(1920, 1080);
        vec3 position = calculatePosition(texCoord);

//        vec3 lighting  = vec3(0.03f) * Albedo * AmbientOcclusion; // hard-coded ambient component
//        vec3 viewDir  = normalize(camPos - WorldPos);
//        vec3 F0 = vec3(0.04);
//        lighting += mix(F0, Albedo, Metallic);

        vec3 lighting = calculateDirLight(dirLight, Normal, FragPos, texCoord);

        for(int i = 0; i < pointNum; i++)
        {
            lighting += calculatePointLight(pointLights[i], Normal, FragPos, texCoord);
        }

        for(int i = 0; i < spotNum; i++)
        {
            SpotLight spot = spotLights[i];
            lighting += calculateSpotLight(spot, Normal, FragPos);
        }

//        vec3 lighting  = vec3(0.03f) * Albedo * AmbientOcclusion; // hard-coded ambient component
//        vec3 viewDir  = normalize(camPos - WorldPos);
//
//        vec3 F0 = vec3(0.04);
//        F0 = mix(F0, Albedo, Metallic);
        //vec3 ambient = vec3(0.03) * Albedo * AmbientOcclusion;
//        for(int i = 0; i < pointNum; i++)
//        {
//            lighting += calculatePointLight(Roughness, Normal, Metallic, Albedo, normalize(-FragPos), F0, pointLights[i], WorldPos);
//        }

        FragColor = vec4(lighting, 1.0);
}

//void main()
//{
//    vec3 FragPos = texture(gPosition, TexCoords).rgb;
//    vec3 Normal = texture(gNormal, TexCoords).rgb;
//    vec3 Albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2f));
//    float Specular = texture(gAlbedo, TexCoords).a;
//    vec3 WorldPos = texture(gWorldPos, TexCoords).rgb;
//    vec3 Emissive = texture(gEmissive, TexCoords).rgb;
//    float Metallic = texture(gMetallicRoughnessAmbient, TexCoords).r;
//    float Roughness = texture(gMetallicRoughnessAmbient, TexCoords).g;
//    float AmbientOcclusion = texture(gMetallicRoughnessAmbient, TexCoords).b;
//
//    vec3 V = normalize(camPos - FragPos);
//
//    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
//    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
//    vec3 F0 = vec3(0.04);
//    F0 = mix(F0, Albedo, Metallic);
//
//    // init reflectance equation
//    vec3 Lo = vec3(0.0);
//
//    // calculate light radiance
//    vec3 L = normalize(pointLight.position - WorldPos);
//    vec3 H = normalize(V + L);
//    float distance = length(pointLight.position - WorldPos);
//    float attenuation = 1.0 / (distance * distance);
//    vec3 radiance = pointLight.color * attenuation;
//
//    // Cook-Torrance BRDF
//    float NDF = DistributionGGX(Normal, H, Roughness);
//    float G   = GeometrySmith(Normal, V, L, Roughness);
//    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
//
//    vec3 numerator    = NDF * G * F;
//    float denominator = 4.0 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
//    vec3 specular = numerator / denominator;
//
//    // kS is equal to Fresnel
//    vec3 kS = F;
//    // energy conservation
//    vec3 kD = vec3(1.0) - kS;
//    // multiply kD by the inverse metalness such that only non-metals
//    // have diffuse lighting, or a linear blend if partly metal (pure metals
//    // have no diffuse light).
//    kD *= 1.0 - Metallic;
//
//    // scale light by NdotL
//    float NdotL = max(dot(Normal, L), 0.0);
//
//    // add to outgoing radiance Lo
//    Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
//
//    // calculate directional light radiance
////    L = normalize(-dirLight.direction);
////    H = normalize(V + L);
////    radiance = dirLight.color;
////
////    // Cook-Torrance BRDF
////    NDF = DistributionGGX(Normal, H, Roughness);
////    G   = GeometrySmith(Normal, V, L, Roughness);
////    F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
////
////    numerator    = NDF * G * F;
////    denominator = 4.0 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
////    specular = numerator / denominator;
////
////    // kS is equal to Fresnel
////    kS = F;
////    // energy conservation
////    kD = vec3(1.0) - kS;
////    // multiply kD by the inverse metalness such that only non-metals
////    // have diffuse lighting, or a linear blend if partly metal (pure metals
////    // have no diffuse light).
////    kD *= 1.0 - Metallic;
////
////    // scale light by NdotL
////    NdotL = max(dot(Normal, L), 0.0);
////
////    // add to outgoing radiance Lo
////    Lo += (kD * Albedo / PI + specular) * radiance * NdotL;
//
//    vec3 ambient = vec3(0.03) * Albedo * AmbientOcclusion;
//
//    vec3 color = ambient + calculateDirLight(V, Normal, Roughness, F0, Metallic, Albedo);
//
//
//    for(int i = 0; i < spotNum; i++)
//    {
//        float spotlightIntensity = calculateSpotLight(spotLight.position, spotLight.direction, spotLight.cutoff, spotLight.outercutoff, Normal, WorldPos, V);
//        color += spotlightIntensity;
//    }
//
//
//    // HDR tonemapping
//    color = color / (color + vec3(1.0));
//    // gamma correct
//    color = pow(color, vec3(1.0/2.2));
//
//    //vec3 finalColor = cel(color);
//    FragColor = vec4(color, 1.0);
//}