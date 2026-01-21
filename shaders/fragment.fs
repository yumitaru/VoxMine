#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec2 vFaceUV;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform int layerY;

uniform sampler2D texStone;
uniform sampler2D texMoss;

float edgeFactorQuad(vec2 uv)
{
    float thickness = 0.05;
    float e = min(min(uv.x, 1.0 - uv.x),
                  min(uv.y, 1.0 - uv.y));
    return smoothstep(0.0, thickness, e);
}

void main()
{
    vec3 baseColor;

    if (layerY >= 1 && layerY <= 16)
    {
        baseColor = vec3(0.2, 1.0, 0.2);
    }
    else
    {
        vec3 stone = texture(texStone, vFaceUV).rgb;
        vec3 moss  = texture(texMoss,  vFaceUV).rgb;

        float mossFactor = clamp(normalize(vNormal).y, 0.0, 1.0);
        baseColor = mix(stone, moss, mossFactor);
    }

    vec3 N = normalize(vNormal);
    vec3 L = normalize(-lightDir);

    float diff = max(dot(N, L), 0.0);

    vec3 lighting = ambientColor + lightColor * diff;
    vec3 color = baseColor * lighting;

    float edge = edgeFactorQuad(vFaceUV);
    color = mix(vec3(0.0), color, edge);

    FragColor = vec4(color, 1.0);
}
