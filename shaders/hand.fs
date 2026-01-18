#version 330 core
out vec4 FragColor;

uniform int mode;

void main()
{
    if (mode == 0)      FragColor = vec4(0.85, 0.7, 0.55, 1.0); // ręka
    else if (mode == 1) FragColor = vec4(0.55, 0.35, 0.15, 1.0); // trzonek
    else               FragColor = vec4(0.6, 0.6, 0.6, 1.0); // kamień
}
