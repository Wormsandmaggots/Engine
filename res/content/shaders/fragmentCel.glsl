#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

float celValue(float value){
    if(value < 0.25) return 0.1;
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

void main()
{
    vec3 result = texture(texture_diffuse1, TexCoords).rgb;
    vec3 result2 = cel(result);
    FragColor = texture(result2, 1.0f);
}