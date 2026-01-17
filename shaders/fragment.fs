#version 330 core
out vec4 FragColor;

in vec2 vFaceUV;

uniform int layerY;

float edgeFactorQuad(vec2 uv)
{
    float thickness = 0.05;

    float e =
        min(min(uv.x, 1.0 - uv.x),
            min(uv.y, 1.0 - uv.y));

    return smoothstep(0.0, thickness, e);
}

void main()
{
    vec3 fillColor;

    if (layerY == -1)
        fillColor = vec3(0.5, 0.5, 0.5);
    else if (layerY == 16)
        fillColor = vec3(0.2, 1.0, 0.2);
    else if (layerY == 15)
        fillColor = vec3(0.2, 0.95, 0.2);
    else if (layerY == 14)
        fillColor = vec3(0.2, 0.9, 0.2);
    else if (layerY == 13)
        fillColor = vec3(0.2, 0.85, 0.2);
    else if (layerY == 12)
        fillColor = vec3(0.2, 0.8, 0.2);
    else if (layerY == 11)
        fillColor = vec3(0.2, 0.75, 0.2);
    else if (layerY == 10)
        fillColor = vec3(0.2, 0.7, 0.2);
    else if (layerY == 9)
        fillColor = vec3(0.2, 0.65, 0.2);
    else if (layerY == 8)
        fillColor = vec3(0.2, 0.6, 0.2);
    else if (layerY == 7)
        fillColor = vec3(0.2, 0.55, 0.2);
    else if (layerY == 6)
        fillColor = vec3(0.2, 0.5, 0.2);
    else if (layerY == 5)
        fillColor = vec3(0.2, 0.45, 0.2);
    else if (layerY == 4)
        fillColor = vec3(0.2, 0.4, 0.2);
    else if (layerY == 3)
        fillColor = vec3(0.2, 0.35, 0.2);
    else if (layerY == 2)
        fillColor = vec3(0.2, 0.3, 0.2);
    else if (layerY == 1)
        fillColor = vec3(0.2, 0.25, 0.2);
    else if (layerY == 999)
        fillColor = vec3(1.0, 0.984, 0);
    else
        fillColor = vec3(0.55, 0.27, 0.07);

    float edge = edgeFactorQuad(vFaceUV);


    FragColor = vec4(mix(vec3(0.0), fillColor, edge), 1.0);
    
}


