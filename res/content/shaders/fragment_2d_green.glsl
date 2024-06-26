#version 460 core
out vec4 FragColor;

uniform float scale;
uniform bool isBarLocked;

void main()
{
    if (isBarLocked) {
        FragColor = vec4(0.5, 0.5, 0.5, 1.0); // szary
    } else {
        if (scale > 0.06 && scale <= 0.15) {
            FragColor = vec4(1.0, 0.5, 0.0, 1.0); // pomarańczowy
        } else if (scale <= 0.06) {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // czerwony
        } else {
            FragColor = vec4(0.0, 1.0, 0.0, 1.0); // zielony
        }
    }
}