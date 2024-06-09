/*If you want to use this shader remember about those uniforms
shaderPbr.use();
shaderPbr.setVec3("camPos",s.camera.Position);
shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());*/
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_metalic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ambient1;
// lights
uniform vec3 camPos;
uniform vec3 lightPos;

//spotlight
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform float cutOff;
uniform float outerCutOff;

// directional light
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

vec3 lightColor = vec3(150.0f, 150.0f, 150.0f);
const float PI = 3.14159265359;

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

float celValue(float value){
    if(value < 0.05) return 0.1;
    if(value < 0.15) return 0.2;
    if(value < 0.25) return 0.3;
    if(value < 0.5) return 0.4;
    if(value < 0.75) return 0.7;
    return 1;
}

vec3 cel(vec3 color){
    color.x = celValue(color.x);
    color.y = celValue(color.y);
    color.z = celValue(color.z);
    return color;
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

    // calculate light radiance
    vec3 L = normalize(lightPos - WorldPos);
    vec3 H = normalize(V + L);
    float distance = length(lightPos - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColor * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
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
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    // calculate directional light radiance
    L = normalize(-dirLightDir);
    H = normalize(V + L);
    radiance = dirLightColor;

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
    float spotlightIntensity = calculateSpotLight(spotLightPos, spotLightDir, cutOff, outerCutOff, N, WorldPos, V);
    color += spotlightIntensity;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    vec3 finalColor = cel(color);
    FragColor = vec4(color, 1.0);
    //FragColor = vec4(finalColor, 1.0); // THIS IF U WANT TO USE CEL SHADING
}