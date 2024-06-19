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
    vec3 albedo     = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(gMetallicRoughnessAmbient, TexCoords).r;
    float roughness = texture(gMetallicRoughnessAmbient, TexCoords).g;
    float ao        = texture(gMetallicRoughnessAmbient, TexCoords).b;

    vec3 V = normalize(camPos - position);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 L = normalize(-lightSource.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = lightSource.colors.diffuse;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, H, roughness);
    float G   = GeometrySmith(normal, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // energy conservation
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(normal, L), 0.0);

    // add to outgoing radiance Lo
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo * ao;

    return ambient + Lo;
}

vec3 calculateDirLight2(DirectionalLight lightSource, vec3 normal, vec3 position, vec2 texCoord)
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

    vec3 norm = normal;
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

//    if(distance > lightSource.radius)
//            return vec3(0);

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
    float spec = pow(max(dot(normal, halfway), 0.0), 8.0);
    vec3 specular = (numerator / denominator) * spec ;

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
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 position = texture(gPosition, TexCoords).rgb;

//    vec3 light = calculateDirLight2(dirLight, normal, texture(gWorldPos, TexCoords).rgb, TexCoords);

//    for(int i = 0; i < pointNum; i++)
//    {
//        light += calculatePointLight(pointLights[i], normal, position, TexCoords);
//    }
//
//    for (int i = 0; i < spotNum; i++)
//    {
//        light += calculateSpotLight(spotLights[i], normal, position);
//    }

    FragColor = vec4(texture(gAlbedo, TexCoords).rgb, 1.0f);
}

//void main()
//{
////        vec3 FragPos = texture(gPosition, TexCoords).rgb;
////        vec3 Normal = texture(gNormal, TexCoords).rgb;
////        vec3 Albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2f));
////        float Specular = texture(gAlbedo, TexCoords).a;
////        vec3 WorldPos = texture(gWorldPos, TexCoords).rgb;
////        vec3 Emissive = texture(gEmissive, TexCoords).rgb;
////        float Metallic = texture(gMetallicRoughnessAmbient, TexCoords).r;
////        float Roughness = texture(gMetallicRoughnessAmbient, TexCoords).g;
////        float AmbientOcclusion = (texture(gMetallicRoughnessAmbient, TexCoords).b + texture(ssao, TexCoords).r);
////
////        vec2 texCoord = gl_FragCoord.xy / vec2(1920, 1080);
////        vec3 position = calculatePosition(texCoord);
////
////        vec3 lighting = calculateDirLight(dirLight, Normal, FragPos, TexCoords);
////
////        FragColor = vec4(lighting, 1.0);
//
//// retrieve data from gbuffer
//vec3 FragPos = texture(gPosition, TexCoords).rgb;
//vec3 Normal = texture(gNormal, TexCoords).rgb;
//vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
//float AmbientOcclusion = texture(ssao, TexCoords).r;
//
//// blinn-phong (in view-space)
//vec3 ambient = vec3(0.3 * Diffuse * AmbientOcclusion); // here we add occlusion factor
//vec3 lighting  = ambient;
//vec3 viewDir  = normalize(-FragPos); // viewpos is (0.0.0) in view-space
//// diffuse
//vec3 lightDir = normalize(pointLights[0].position - FragPos);
//vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * pointLights[0].colors.diffuse;
//// specular
//vec3 halfwayDir = normalize(lightDir + viewDir);
//float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
//vec3 specular = pointLights[0].colors.diffuse * spec;
//// attenuation
//float dist = length(pointLights[0].position - FragPos);
//float attenuation = 1.0 / (1.0 + pointLights[0].linear * dist + pointLights[0].quadratic * dist * dist);
//diffuse  *= attenuation;
//specular *= attenuation;
//lighting += diffuse + specular;
//
//FragColor = vec4(lighting, 1.0);
//}

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