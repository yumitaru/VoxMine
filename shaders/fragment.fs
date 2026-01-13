#version 330 core
out vec4 FragColor;

uniform int layerY;

void main()
{
    if (layerY == 4)
        FragColor = vec4(0.55, 0.27, 0.07, 1.0);
    else if (layerY == 3)
        FragColor = vec4(0.6, 0.4, 0.2, 1.0);
    else if (layerY == 2)
        FragColor = vec4(0.5, 0.5, 0.5, 1.0);
    else if (layerY == 1)
        FragColor = vec4(0.3, 0.3, 0.3, 1.0);
    else
        FragColor = vec4(0.1, 0.1, 0.1, 1.0);
}
