#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D tex;
uniform int mode;
// 0 = ręka
// 1 = narzędzie

void main()
{
    if (mode == 1) {
        FragColor = texture(tex, vUV);
    } else {
        FragColor = vec4(0.85, 0.75, 0.6, 1.0);
    }
}
