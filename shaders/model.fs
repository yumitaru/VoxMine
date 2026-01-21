#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D diffuseTex;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main()
{
    vec3 baseColor = texture(diffuseTex, vTexCoord).rgb;

    vec3 N = normalize(vNormal);
    vec3 L = normalize(-lightDir);

    float diff = max(dot(N, L), 0.0);
    vec3 lighting = ambientColor + lightColor * diff;



    FragColor = vec4(baseColor * lighting, 1.0);

}
