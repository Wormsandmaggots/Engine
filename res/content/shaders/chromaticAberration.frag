#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float time;

void main()
{
    vec2 uv = TexCoords;

    float amount = 0.0;

    amount = (1.0 + sin(time*6.0)) * 0.5;
    amount *= 1.0 + sin(time*16.0) * 0.5;
    amount *= 1.0 + sin(time*19.0) * 0.5;
    amount *= 1.0 + sin(time*27.0) * 0.5;
    amount = pow(amount, 3.0);

    amount *= 0.05;

    vec3 col;
    col.r = texture( screenTexture, vec2(uv.x+amount,uv.y) ).r;
    col.g = texture( screenTexture, uv ).g;
    col.b = texture( screenTexture, vec2(uv.x-amount,uv.y) ).b;

    col *= (1.0 - amount * 0.5);

    FragColor = vec4(col,1.0);
}