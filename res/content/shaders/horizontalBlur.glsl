#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;
uniform float blurSize;

const int MAX_KERNEL_SIZE = 25;
uniform float kernel[MAX_KERNEL_SIZE];
uniform float offset[MAX_KERNEL_SIZE];

void main()
{
    vec3 result = texture(image, TexCoords).rgb * kernel[0];
    for(int i = 1; i < MAX_KERNEL_SIZE; ++i)
    {
        result += texture(image, TexCoords + vec2(offset[i] * blurSize, 0.0)).rgb * kernel[i];
        result += texture(image, TexCoords - vec2(offset[i] * blurSize, 0.0)).rgb * kernel[i];
    }
    FragColor = vec4(result, 1.0);
}